/**
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * @author               Anne van Rossum
 * @copyright            Distributed Organisms B.V. (DoBots)
 * @date                 Mar 23, 2015
 * @license              LGPLv3
 */

#include "RecommenderModule.h"
#include <RecommenderModuleExt.h>

namespace rur {
using namespace v8;

RecommenderModule::RecommenderModule():
  cliParam(0)
{
  const char* const channel[3] = {"readDocument", "readTerm", "writeRecommendation"};
  cliParam = new Param();
  DestroyFlag = false;
  readBufDocument = std::deque<std::string>(0);
  readBufTerm = std::deque<std::string>(0);
  writeBufRecommendation = std::deque<std::string>(0);
}

RecommenderModule::~RecommenderModule() {
  delete cliParam;
}

void RecommenderModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

static void* RunModule(void* object) {
  static_cast<RecommenderModuleExt*>(object)->Run();
}

void RecommenderModule::Run() {
  while (true) {
    Tick();
  }
}

v8::Handle<v8::Value> RecommenderModule::NodeNew(const v8::Arguments& args) {
  v8::HandleScope scope;
  if ((args.Length() < 1) || (!args[0]->IsString())) {
    return v8::ThrowException(v8::String::New("Invalid argument, must provide a string."));
  }
  RecommenderModuleExt* obj = new RecommenderModuleExt();
  obj->Wrap(args.This());
  
  v8::String::Utf8Value v8str(args[0]->ToString());
  std::string name = std::string(*v8str);
  obj->Init(name);
  
  pthread_mutex_init(&(obj->destroyMutex), NULL);
  
  // Init ports
  pthread_mutex_init(&(obj->readMutexDocument), NULL);
  
  pthread_mutex_init(&(obj->readMutexTerm), NULL);
  
  pthread_mutex_init(&(obj->writeMutexRecommendation), NULL);
  uv_async_init(uv_default_loop() , &(obj->asyncRecommendation), &(obj->CallBackRecommendation));
  
  // Start the module loop
  pthread_create(&(obj->moduleThread), 0, RunModule, obj);
  
  // Make this object persistent
  obj->Ref();
  
  return args.This();
}

v8::Handle<v8::Value> RecommenderModule::NodeDestroy(const v8::Arguments& args) {
  v8::HandleScope scope;
  RecommenderModuleExt* obj = ObjectWrap::Unwrap<RecommenderModuleExt>(args.This());
  return scope.Close(v8::Boolean::New(obj->Destroy()));
}

bool RecommenderModule::Destroy() {
  bool canDestroy = true;
  if (canDestroy) {
    pthread_mutex_lock(&writeMutexRecommendation);
    if (!writeBufRecommendation.empty())
      canDestroy = false;
    pthread_mutex_unlock(&writeMutexRecommendation);
  }
  if (canDestroy) {
    pthread_cancel(moduleThread);
    Unref();
    return true;
  }
  else {
    pthread_mutex_lock(&destroyMutex);
    DestroyFlag = true;
    pthread_mutex_unlock(&destroyMutex);
    return true; // return true anyway?
  }
}

void RecommenderModule::NodeRegister(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(NodeNew);
  tpl->SetClassName(v8::String::NewSymbol("RecommenderModule"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  
  // Prototypes
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("Destroy"), v8::FunctionTemplate::New(NodeDestroy)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("WriteDocument"), v8::FunctionTemplate::New(NodeWriteDocument)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("WriteTerm"), v8::FunctionTemplate::New(NodeWriteTerm)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("RegReadRecommendation"), v8::FunctionTemplate::New(NodeRegReadRecommendation)->GetFunction());
  
  v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("RecommenderModule"), constructor);
}

v8::Handle<v8::Value> RecommenderModule::NodeWriteDocument(const v8::Arguments& args) {
  v8::HandleScope scope;
  RecommenderModuleExt* obj = ObjectWrap::Unwrap<RecommenderModuleExt>(args.This());
  if (args.Length() < 1)
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  pthread_mutex_lock(&(obj->readMutexDocument));
  v8::String::Utf8Value v8str(args[0]->ToString());
  obj->readBufDocument.push_back(std::string(*v8str));
  pthread_mutex_unlock(&(obj->readMutexDocument));
  return scope.Close(v8::Boolean::New(true));
}

std::string* RecommenderModule::readDocument(bool blocking) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return NULL;
  pthread_mutex_lock(&readMutexDocument);
  if (readBufDocument.empty()) {
    pthread_mutex_unlock(&readMutexDocument); // Don't forget to unlock!
    return NULL;
  }
  readValDocument = readBufDocument.front();
  readBufDocument.pop_front();
  pthread_mutex_unlock(&readMutexDocument);
  return &readValDocument;
}

v8::Handle<v8::Value> RecommenderModule::NodeWriteTerm(const v8::Arguments& args) {
  v8::HandleScope scope;
  RecommenderModuleExt* obj = ObjectWrap::Unwrap<RecommenderModuleExt>(args.This());
  if (args.Length() < 1)
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  pthread_mutex_lock(&(obj->readMutexTerm));
  v8::String::Utf8Value v8str(args[0]->ToString());
  obj->readBufTerm.push_back(std::string(*v8str));
  pthread_mutex_unlock(&(obj->readMutexTerm));
  return scope.Close(v8::Boolean::New(true));
}

std::string* RecommenderModule::readTerm(bool blocking) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return NULL;
  pthread_mutex_lock(&readMutexTerm);
  if (readBufTerm.empty()) {
    pthread_mutex_unlock(&readMutexTerm); // Don't forget to unlock!
    return NULL;
  }
  readValTerm = readBufTerm.front();
  readBufTerm.pop_front();
  pthread_mutex_unlock(&readMutexTerm);
  return &readValTerm;
}

v8::Handle<v8::Value> RecommenderModule::NodeRegReadRecommendation(const v8::Arguments& args) {
  v8::HandleScope scope;
  RecommenderModuleExt* obj = ObjectWrap::Unwrap<RecommenderModuleExt>(args.This());
  if (args.Length() < 1 || !args[0]->IsFunction())
    return scope.Close(v8::Boolean::New(false)); // Could also throw an exception
  v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[0]);
  obj->nodeCallBackRecommendation = v8::Persistent<v8::Function>::New(callback);
  return scope.Close(v8::Boolean::New(true));
}

void RecommenderModule::CallBackRecommendation(uv_async_t *handle, int status) {
  v8::HandleScope scope;
  RecommenderModuleExt* obj = (RecommenderModuleExt*)(handle->data);
  const unsigned argc = 1;
  while (true) {
    pthread_mutex_lock(&(obj->writeMutexRecommendation));
    if (obj->writeBufRecommendation.empty()) {
      pthread_mutex_unlock(&(obj->writeMutexRecommendation)); // Don't forget to unlock!
      break;
    }
    v8::Local<v8::Value> argv[argc] = { v8::Local<v8::Value>::New(v8::String::New(obj->writeBufRecommendation.front().c_str())) };
    obj->writeBufRecommendation.pop_front();
    pthread_mutex_unlock(&(obj->writeMutexRecommendation));
    if (!obj->nodeCallBackRecommendation.IsEmpty())
      obj->nodeCallBackRecommendation->Call(v8::Context::GetCurrent()->Global(), argc, argv);
  }
  pthread_mutex_lock(&(obj->destroyMutex));
  bool destroy = obj->DestroyFlag;
  pthread_mutex_unlock(&(obj->destroyMutex));
  if (destroy)
    obj->Destroy();
}

bool RecommenderModule::writeRecommendation(const std::string output) {
  pthread_mutex_lock(&destroyMutex);
  bool destroy = DestroyFlag;
  pthread_mutex_unlock(&destroyMutex);
  if (destroy)
    return false;
  pthread_mutex_lock(&writeMutexRecommendation);
  writeBufRecommendation.push_back(output);
  pthread_mutex_unlock(&writeMutexRecommendation);
  asyncRecommendation.data = (void*) this;
  uv_async_send(&asyncRecommendation);
  return true;
}

} // namespace

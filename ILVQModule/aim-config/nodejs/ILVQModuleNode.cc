#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <ILVQModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  ILVQModuleExt::NodeRegister(exports);
}

NODE_MODULE(ILVQModule, RegisterModule)

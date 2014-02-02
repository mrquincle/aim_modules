#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <NaiveBayesModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  NaiveBayesModuleExt::NodeRegister(exports);
}

NODE_MODULE(NaiveBayesModule, RegisterModule)

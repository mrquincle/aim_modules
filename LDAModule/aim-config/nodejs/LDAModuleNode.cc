#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <LDAModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  LDAModuleExt::NodeRegister(exports);
}

NODE_MODULE(LDAModule, RegisterModule)

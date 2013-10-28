#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <HorizonModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  HorizonModuleExt::NodeRegister(exports);
}

NODE_MODULE(HorizonModule, RegisterModule)

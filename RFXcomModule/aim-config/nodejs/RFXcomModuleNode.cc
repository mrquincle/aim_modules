#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <RFXcomModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  RFXcomModuleExt::NodeRegister(exports);
}

NODE_MODULE(RFXcomModule, RegisterModule)

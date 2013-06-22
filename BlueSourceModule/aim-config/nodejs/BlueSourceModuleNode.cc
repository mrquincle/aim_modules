#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <BlueSourceModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  BlueSourceModuleExt::NodeRegister(exports);
}

NODE_MODULE(BlueSourceModule, RegisterModule)

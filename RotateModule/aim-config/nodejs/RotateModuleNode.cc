#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <RotateModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  RotateModuleExt::NodeRegister(exports);
}

NODE_MODULE(RotateModule, RegisterModule)

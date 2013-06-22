#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <MovingAverageModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  MovingAverageModuleExt::NodeRegister(exports);
}

NODE_MODULE(MovingAverageModule, RegisterModule)

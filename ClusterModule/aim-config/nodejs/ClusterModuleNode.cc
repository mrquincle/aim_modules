#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <ClusterModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  ClusterModuleExt::NodeRegister(exports);
}

NODE_MODULE(ClusterModule, RegisterModule)

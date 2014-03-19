#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <DatasetModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  DatasetModuleExt::NodeRegister(exports);
}

NODE_MODULE(DatasetModule, RegisterModule)

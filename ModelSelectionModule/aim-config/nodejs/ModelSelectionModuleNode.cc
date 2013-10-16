#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <ModelSelectionModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  ModelSelectionModuleExt::NodeRegister(exports);
}

NODE_MODULE(ModelSelectionModule, RegisterModule)

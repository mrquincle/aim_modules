#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <DirichletModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  DirichletModuleExt::NodeRegister(exports);
}

NODE_MODULE(DirichletModule, RegisterModule)

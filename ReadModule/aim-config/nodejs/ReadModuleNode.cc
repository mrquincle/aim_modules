#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <ReadModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  ReadModuleExt::NodeRegister(exports);
}

NODE_MODULE(ReadModule, RegisterModule)

#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <RecommenderModuleExt.h>

using namespace v8;
using namespace rur;

void RegisterModule(Handle<Object> exports) {
  RecommenderModuleExt::NodeRegister(exports);
}

NODE_MODULE(RecommenderModule, RegisterModule)

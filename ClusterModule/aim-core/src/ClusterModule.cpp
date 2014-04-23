/**
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * @author               Anne C. van Rossum
 * @copyright            Distributed Organisms B.V.
 * @date                 Nov 18, 2013
 * @license              GNU General Lesser Public
 */

#include "ClusterModule.h"

namespace rur {
using namespace yarp::os;

ClusterModule::ClusterModule():
  cliParam(0)
{
  const char* const channel[5] = {"readTrain", "readTest", "writeClass", "readClusterCount", "readMethod"};
  cliParam = new Param();
  portTrain = new BufferedPort<Bottle>();
  portTest = new BufferedPort<Bottle>();
  portClass = new BufferedPort<Bottle>();
  portClusterCount = new BufferedPort<Bottle>();
  portMethod = new BufferedPort<Bottle>();
}

ClusterModule::~ClusterModule() {
  delete cliParam;
  delete portTrain;
  delete portTest;
  delete portClass;
  delete portClusterCount;
  delete portMethod;
}

void ClusterModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/clustermodule" << name << "/train";
  portTrain->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/clustermodule" << name << "/test";
  portTest->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/clustermodule" << name << "/class";
  portClass->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/clustermodule" << name << "/clustercount";
  portClusterCount->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/clustermodule" << name << "/method";
  portMethod->open(yarpPortName.str().c_str());
  
}

long_seq* ClusterModule::readTrain(bool blocking) {
  Bottle *b = portTrain->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portTrainBuf.push_back(b->get(i).asInt());
    }
  }
  return &portTrainBuf;
}

long_seq* ClusterModule::readTest(bool blocking) {
  Bottle *b = portTest->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portTestBuf.push_back(b->get(i).asInt());
    }
  }
  return &portTestBuf;
}

bool ClusterModule::writeClass(const int output) {
  Bottle &outputPrepare = portClass->prepare();
  outputPrepare.clear();
  outputPrepare.addInt(output);
  bool forceStrict = true; // wait till previous sends are complete
  portClass->write(forceStrict);
  return true;
}

int* ClusterModule::readClusterCount(bool blocking) {
  Bottle *b = portClusterCount->read(blocking);
  if (b != NULL) { 
    portClusterCountBuf = b->get(0).asInt();
    return &portClusterCountBuf;
  }
  return NULL;
}

int* ClusterModule::readMethod(bool blocking) {
  Bottle *b = portMethod->read(blocking);
  if (b != NULL) { 
    portMethodBuf = b->get(0).asInt();
    return &portMethodBuf;
  }
  return NULL;
}

} // namespace

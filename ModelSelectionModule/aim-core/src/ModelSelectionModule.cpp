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
 * @date                 Oct 18, 2013
 * @license              GNU General Lesser Public
 */

#include "ModelSelectionModule.h"

namespace rur {
using namespace yarp::os;

ModelSelectionModule::ModelSelectionModule():
  cliParam(0)
{
  const char* const channel[4] = {"readResiduals", "readModelParameterCount", "readMethod", "writeModelQuality"};
  cliParam = new Param();
  portResiduals = new BufferedPort<Bottle>();
  portModelParameterCount = new BufferedPort<Bottle>();
  portMethod = new BufferedPort<Bottle>();
  portModelQuality = new BufferedPort<Bottle>();
}

ModelSelectionModule::~ModelSelectionModule() {
  delete cliParam;
  delete portResiduals;
  delete portModelParameterCount;
  delete portMethod;
  delete portModelQuality;
}

void ModelSelectionModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/modelselectionmodule" << name << "/residuals";
  portResiduals->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/modelselectionmodule" << name << "/modelparametercount";
  portModelParameterCount->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/modelselectionmodule" << name << "/method";
  portMethod->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/modelselectionmodule" << name << "/modelquality";
  portModelQuality->open(yarpPortName.str().c_str());
  
}

long_seq* ModelSelectionModule::readResiduals(bool blocking) {
  Bottle *b = portResiduals->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portResidualsBuf.push_back(b->get(i).asInt());
    }
  }
  return &portResidualsBuf;
}

int* ModelSelectionModule::readModelParameterCount(bool blocking) {
  Bottle *b = portModelParameterCount->read(blocking);
  if (b != NULL) { 
    portModelParameterCountBuf = b->get(0).asInt();
    return &portModelParameterCountBuf;
  }
  return NULL;
}

int* ModelSelectionModule::readMethod(bool blocking) {
  Bottle *b = portMethod->read(blocking);
  if (b != NULL) { 
    portMethodBuf = b->get(0).asInt();
    return &portMethodBuf;
  }
  return NULL;
}

bool ModelSelectionModule::writeModelQuality(const float output) {
  Bottle &outputPrepare = portModelQuality->prepare();
  outputPrepare.clear();
  outputPrepare.addDouble(output);
  bool forceStrict = true; // wait till previous sends are complete
  portModelQuality->write(forceStrict);
  return true;
}

} // namespace

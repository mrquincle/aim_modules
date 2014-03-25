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
 * @date                 Mar 24, 2014
 * @license              GNU General Lesser Public
 */

#include "LDAModule.h"

namespace rur {
using namespace yarp::os;

LDAModule::LDAModule():
  cliParam(0)
{
  const char* const channel[5] = {"readSample", "readMode", "readLag", "readConvergence", "writeLeftWheel"};
  cliParam = new Param();
  portSample = new BufferedPort<Bottle>();
  portMode = new BufferedPort<Bottle>();
  portLag = new BufferedPort<Bottle>();
  portConvergence = new BufferedPort<Bottle>();
  portLeftWheel = new BufferedPort<Bottle>();
}

LDAModule::~LDAModule() {
  delete cliParam;
  delete portSample;
  delete portMode;
  delete portLag;
  delete portConvergence;
  delete portLeftWheel;
}

void LDAModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/ldamodule" << name << "/sample";
  portSample->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/ldamodule" << name << "/mode";
  portMode->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/ldamodule" << name << "/lag";
  portLag->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/ldamodule" << name << "/convergence";
  portConvergence->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/ldamodule" << name << "/leftwheel";
  portLeftWheel->open(yarpPortName.str().c_str());
  
}

long_seq* LDAModule::readSample(bool blocking) {
  Bottle *b = portSample->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portSampleBuf.push_back(b->get(i).asInt());
    }
  }
  return &portSampleBuf;
}

int* LDAModule::readMode(bool blocking) {
  Bottle *b = portMode->read(blocking);
  if (b != NULL) { 
    portModeBuf = b->get(0).asInt();
    return &portModeBuf;
  }
  return NULL;
}

int* LDAModule::readLag(bool blocking) {
  Bottle *b = portLag->read(blocking);
  if (b != NULL) { 
    portLagBuf = b->get(0).asInt();
    return &portLagBuf;
  }
  return NULL;
}

double* LDAModule::readConvergence(bool blocking) {
  Bottle *b = portConvergence->read(blocking);
  if (b != NULL) { 
    portConvergenceBuf = b->get(0).asDouble();
    return &portConvergenceBuf;
  }
  return NULL;
}

bool LDAModule::writeLeftWheel(const int output) {
  Bottle &outputPrepare = portLeftWheel->prepare();
  outputPrepare.clear();
  outputPrepare.addInt(output);
  bool forceStrict = true; // wait till previous sends are complete
  portLeftWheel->write(forceStrict);
  return true;
}

} // namespace

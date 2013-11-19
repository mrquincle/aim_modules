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
 * @date                 Oct 22, 2013
 * @license              GNU General Lesser Public
 */

#include "DirichletModule.h"

namespace rur {
using namespace yarp::os;

DirichletModule::DirichletModule():
  cliParam(0)
{
  const char* const channel[3] = {"readAudio", "readInfrared", "writeLeftWheel"};
  cliParam = new Param();
  portAudio = new BufferedPort<Bottle>();
  portInfrared = new BufferedPort<Bottle>();
  portLeftWheel = new BufferedPort<Bottle>();
}

DirichletModule::~DirichletModule() {
  delete cliParam;
  delete portAudio;
  delete portInfrared;
  delete portLeftWheel;
}

void DirichletModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/dirichletmodule" << name << "/audio";
  portAudio->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/dirichletmodule" << name << "/infrared";
  portInfrared->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/dirichletmodule" << name << "/leftwheel";
  portLeftWheel->open(yarpPortName.str().c_str());
  
}

long_seq* DirichletModule::readAudio(bool blocking) {
  Bottle *b = portAudio->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portAudioBuf.push_back(b->get(i).asInt());
    }
  }
  return &portAudioBuf;
}

int* DirichletModule::readInfrared(bool blocking) {
  Bottle *b = portInfrared->read(blocking);
  if (b != NULL) { 
    portInfraredBuf = b->get(0).asInt();
    return &portInfraredBuf;
  }
  return NULL;
}

bool DirichletModule::writeLeftWheel(const int output) {
  Bottle &outputPrepare = portLeftWheel->prepare();
  outputPrepare.clear();
  outputPrepare.addInt(output);
  bool forceStrict = true; // wait till previous sends are complete
  portLeftWheel->write(forceStrict);
  return true;
}

} // namespace

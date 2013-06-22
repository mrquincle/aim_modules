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
 */

#include "MovingAverageModule.h"

using namespace rur;
using namespace yarp::os;

MovingAverageModule::MovingAverageModule():
  cliParam(0)
{
  const char* const channel[2] = {"readInput", "writeAverage"};
  cliParam = new Param();
  portInput = new BufferedPort<Bottle>();
  portAverage = new BufferedPort<Bottle>();
}

MovingAverageModule::~MovingAverageModule() {
  delete cliParam;
  delete portInput;
  delete portAverage;
}

void MovingAverageModule::Init(std::string & name) {
  cliParam->module_id = name;
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/movingaveragemodule" << name << "/input";
  portInput->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/movingaveragemodule" << name << "/average";
  portAverage->open(yarpPortName.str().c_str());
  
}

int* MovingAverageModule::readInput(bool blocking) {
  Bottle *b = portInput->read(blocking);
  if (b != NULL) { 
    portInputValue = b->get(0).asInt();
    return &portInputValue;
  }
  return NULL;
}

bool MovingAverageModule::writeAverage(const int output) {
  Bottle &outputPrepare = portAverage->prepare();
  outputPrepare.clear();
  outputPrepare.addInt(output);
  bool forceStrict = true; // wait till previous sends are complete
  portAverage->write(forceStrict);
  return true;
}


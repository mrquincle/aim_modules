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

MovingAverageModule::MovingAverageModule():
  cliParam(0)
{
  const char* const channel[2] = {"readInput", "writeAverage"};
  cliParam = new Param();
  dummyInput = int(0);
}

MovingAverageModule::~MovingAverageModule() {
  delete cliParam;
}

void MovingAverageModule::Init(std::string & name) {
  cliParam->module_id = name;
}

int* MovingAverageModule::readInput(bool blocking) {
  return &dummyInput;
}

bool MovingAverageModule::writeAverage(const int output) {
  return true;
}


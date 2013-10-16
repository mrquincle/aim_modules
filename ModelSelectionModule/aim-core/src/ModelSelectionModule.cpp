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
 * @author You
 * @copyright Your Company
 * @date 21 Jun. 2013
 * @license LGPLv3
 */

#include "ModelSelectionModule.h"

namespace rur {

ModelSelectionModule::ModelSelectionModule():
  cliParam(0)
{
  const char* const channel[3] = {"readAudio", "readInfrared", "writeLeftWheel"};
  cliParam = new Param();
  dummyAudio = long_seq(0);
  dummyInfrared = int(0);
}

ModelSelectionModule::~ModelSelectionModule() {
  delete cliParam;
}

void ModelSelectionModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

long_seq* ModelSelectionModule::readAudio(bool blocking) {
  return &dummyAudio;
}

int* ModelSelectionModule::readInfrared(bool blocking) {
  return &dummyInfrared;
}

bool ModelSelectionModule::writeLeftWheel(const int output) {
  return true;
}

} // namespace

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

ModelSelectionModule::ModelSelectionModule():
  cliParam(0)
{
  const char* const channel[4] = {"readResiduals", "readModelParameterCount", "readMethod", "writeModelQuality"};
  cliParam = new Param();
  dummyResiduals = long_seq(0);
  dummyModelParameterCount = int(0);
  dummyMethod = int(0);
}

ModelSelectionModule::~ModelSelectionModule() {
  delete cliParam;
}

void ModelSelectionModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

long_seq* ModelSelectionModule::readResiduals(bool blocking) {
  return &dummyResiduals;
}

int* ModelSelectionModule::readModelParameterCount(bool blocking) {
  return &dummyModelParameterCount;
}

int* ModelSelectionModule::readMethod(bool blocking) {
  return &dummyMethod;
}

bool ModelSelectionModule::writeModelQuality(const float output) {
  return true;
}

} // namespace

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
 * @date                 Oct 29, 2013
 * @license              GNU General Lesser Public
 */

#include "ILVQModule.h"

namespace rur {

ILVQModule::ILVQModule():
  cliParam(0)
{
  const char* const channel[3] = {"readTraining", "readTesting", "writeResult"};
  cliParam = new Param();
  dummyTraining = long_seq(0);
  dummyTesting = long_seq(0);
}

ILVQModule::~ILVQModule() {
  delete cliParam;
}

void ILVQModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

long_seq* ILVQModule::readTraining(bool blocking) {
  return &dummyTraining;
}

long_seq* ILVQModule::readTesting(bool blocking) {
  return &dummyTesting;
}

bool ILVQModule::writeResult(const int value) {
  return true;
}

} // namespace

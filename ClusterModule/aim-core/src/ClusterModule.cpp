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

ClusterModule::ClusterModule():
  cliParam(0)
{
  const char* const channel[3] = {"readTrain", "readTest", "writeClass"};
  cliParam = new Param();
  dummyTrain = long_seq(0);
  dummyTest = long_seq(0);
}

ClusterModule::~ClusterModule() {
  delete cliParam;
}

void ClusterModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

long_seq* ClusterModule::readTrain(bool blocking) {
  return &dummyTrain;
}

long_seq* ClusterModule::readTest(bool blocking) {
  return &dummyTest;
}

bool ClusterModule::writeClass(const int output) {
  return true;
}

} // namespace

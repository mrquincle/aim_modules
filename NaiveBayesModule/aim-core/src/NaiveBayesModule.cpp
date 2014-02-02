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
 * @date                 Jan 31, 2014
 * @license              GNU General Lesser Public
 */

#include "NaiveBayesModule.h"

namespace rur {

NaiveBayesModule::NaiveBayesModule():
  cliParam(0)
{
  const char* const channel[4] = {"readTrain", "readTest", "writeClass", "writeTestResult"};
  cliParam = new Param();
  dummyTrain = long_seq(0);
  dummyTest = long_seq(0);
}

NaiveBayesModule::~NaiveBayesModule() {
  delete cliParam;
}

void NaiveBayesModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

long_seq* NaiveBayesModule::readTrain(bool blocking) {
  return &dummyTrain;
}

long_seq* NaiveBayesModule::readTest(bool blocking) {
  return &dummyTest;
}

bool NaiveBayesModule::writeClass(const int output) {
  return true;
}

bool NaiveBayesModule::writeTestResult(const long_seq &output) {
  return true;
}

} // namespace

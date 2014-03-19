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
using namespace yarp::os;

NaiveBayesModule::NaiveBayesModule():
  cliParam(0)
{
  const char* const channel[4] = {"readTrain", "readTest", "writeClass", "writeTestResult"};
  cliParam = new Param();
  portTrain = new BufferedPort<Bottle>();
  portTest = new BufferedPort<Bottle>();
  portClass = new BufferedPort<Bottle>();
  portTestResult = new BufferedPort<Bottle>();
}

NaiveBayesModule::~NaiveBayesModule() {
  delete cliParam;
  delete portTrain;
  delete portTest;
  delete portClass;
  delete portTestResult;
}

void NaiveBayesModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/naivebayesmodule" << name << "/train";
  portTrain->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/naivebayesmodule" << name << "/test";
  portTest->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/naivebayesmodule" << name << "/class";
  portClass->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/naivebayesmodule" << name << "/testresult";
  portTestResult->open(yarpPortName.str().c_str());
  
}

long_seq* NaiveBayesModule::readTrain(bool blocking) {
  Bottle *b = portTrain->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portTrainBuf.push_back(b->get(i).asInt());
    }
  }
  return &portTrainBuf;
}

long_seq* NaiveBayesModule::readTest(bool blocking) {
  Bottle *b = portTest->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portTestBuf.push_back(b->get(i).asInt());
    }
  }
  return &portTestBuf;
}

bool NaiveBayesModule::writeClass(const int output) {
  Bottle &outputPrepare = portClass->prepare();
  outputPrepare.clear();
  outputPrepare.addInt(output);
  bool forceStrict = true; // wait till previous sends are complete
  portClass->write(forceStrict);
  return true;
}

bool NaiveBayesModule::writeTestResult(const long_seq &output) {
  Bottle &outputPrepare = portTestResult->prepare();
  outputPrepare.clear();
  for (int i = 0; i < output.size(); ++i) {
    outputPrepare.addInt(output[i]);
  }
  bool forceStrict = true; // wait till previous sends are complete
  portTestResult->write(forceStrict);
  return true;
}

} // namespace

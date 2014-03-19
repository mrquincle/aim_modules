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
 * @date                 Mar 19, 2014
 * @license              GNU General Lesser Public
 */

#include "DatasetModule.h"

namespace rur {
using namespace yarp::os;

DatasetModule::DatasetModule():
  cliParam(0)
{
  const char* const channel[3] = {"writeTestSexClassification", "writeTrainSexClassification", "readExpectedPosterior"};
  cliParam = new Param();
  portTestSexClassification = new BufferedPort<Bottle>();
  portTrainSexClassification = new BufferedPort<Bottle>();
  portExpectedPosterior = new BufferedPort<Bottle>();
}

DatasetModule::~DatasetModule() {
  delete cliParam;
  delete portTestSexClassification;
  delete portTrainSexClassification;
  delete portExpectedPosterior;
}

void DatasetModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/datasetmodule" << name << "/testsexclassification";
  portTestSexClassification->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/datasetmodule" << name << "/trainsexclassification";
  portTrainSexClassification->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/datasetmodule" << name << "/expectedposterior";
  portExpectedPosterior->open(yarpPortName.str().c_str());
  
}

bool DatasetModule::writeTestSexClassification(const long_seq &test_output) {
  Bottle &test_outputPrepare = portTestSexClassification->prepare();
  test_outputPrepare.clear();
  for (int i = 0; i < test_output.size(); ++i) {
    test_outputPrepare.addInt(test_output[i]);
  }
  bool forceStrict = true; // wait till previous sends are complete
  portTestSexClassification->write(forceStrict);
  return true;
}

bool DatasetModule::writeTrainSexClassification(const long_seq &train_output) {
  Bottle &train_outputPrepare = portTrainSexClassification->prepare();
  train_outputPrepare.clear();
  for (int i = 0; i < train_output.size(); ++i) {
    train_outputPrepare.addInt(train_output[i]);
  }
  bool forceStrict = true; // wait till previous sends are complete
  portTrainSexClassification->write(forceStrict);
  return true;
}

int* DatasetModule::readExpectedPosterior(bool blocking) {
  Bottle *b = portExpectedPosterior->read(blocking);
  if (b != NULL) { 
    portExpectedPosteriorBuf = b->get(0).asInt();
    return &portExpectedPosteriorBuf;
  }
  return NULL;
}

} // namespace

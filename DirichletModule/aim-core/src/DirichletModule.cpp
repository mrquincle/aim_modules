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
  const char* const channel[6] = {"readHyperparameters", "readObservation", "readDoTrain", "writeClass", "readGenerate", "writeCRP"};
  cliParam = new Param();
  portHyperparameters = new BufferedPort<Bottle>();
  portObservation = new BufferedPort<Bottle>();
  portDoTrain = new BufferedPort<Bottle>();
  portClass = new BufferedPort<Bottle>();
  portGenerate = new BufferedPort<Bottle>();
  portCRP = new BufferedPort<Bottle>();
}

DirichletModule::~DirichletModule() {
  delete cliParam;
  delete portHyperparameters;
  delete portObservation;
  delete portDoTrain;
  delete portClass;
  delete portGenerate;
  delete portCRP;
}

void DirichletModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/dirichletmodule" << name << "/hyperparameters";
  portHyperparameters->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/dirichletmodule" << name << "/observation";
  portObservation->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/dirichletmodule" << name << "/dotrain";
  portDoTrain->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/dirichletmodule" << name << "/class";
  portClass->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/dirichletmodule" << name << "/generate";
  portGenerate->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/dirichletmodule" << name << "/crp";
  portCRP->open(yarpPortName.str().c_str());
  
}

long_seq* DirichletModule::readHyperparameters(bool blocking) {
  Bottle *b = portHyperparameters->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portHyperparametersBuf.push_back(b->get(i).asInt());
    }
  }
  return &portHyperparametersBuf;
}

float_seq* DirichletModule::readObservation(bool blocking) {
  Bottle *b = portObservation->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portObservationBuf.push_back(b->get(i).asDouble());
    }
  }
  return &portObservationBuf;
}

int* DirichletModule::readDoTrain(bool blocking) {
  Bottle *b = portDoTrain->read(blocking);
  if (b != NULL) { 
    portDoTrainBuf = b->get(0).asInt();
    return &portDoTrainBuf;
  }
  return NULL;
}

bool DirichletModule::writeClass(const long_seq &output) {
  Bottle &outputPrepare = portClass->prepare();
  outputPrepare.clear();
  for (int i = 0; i < output.size(); ++i) {
    outputPrepare.addInt(output[i]);
  }
  bool forceStrict = true; // wait till previous sends are complete
  portClass->write(forceStrict);
  return true;
}

int* DirichletModule::readGenerate(bool blocking) {
  Bottle *b = portGenerate->read(blocking);
  if (b != NULL) { 
    portGenerateBuf = b->get(0).asInt();
    return &portGenerateBuf;
  }
  return NULL;
}

bool DirichletModule::writeCRP(const long_seq &output) {
  Bottle &outputPrepare = portCRP->prepare();
  outputPrepare.clear();
  for (int i = 0; i < output.size(); ++i) {
    outputPrepare.addInt(output[i]);
  }
  bool forceStrict = true; // wait till previous sends are complete
  portCRP->write(forceStrict);
  return true;
}

} // namespace

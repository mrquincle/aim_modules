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
 * @date                 Mar 31, 2015
 * @license              GNU General Lesser Public
 */

#include "DirichletModule.h"

namespace rur {

DirichletModule::DirichletModule():
  cliParam(0)
{
  const char* const channel[6] = {"readHyperparameters", "readObservation", "readDoTrain", "writeClass", "readGenerate", "writeCRP"};
  cliParam = new Param();
  dummyHyperparameters = long_seq(0);
  dummyObservation = float_seq(0);
  dummyDoTrain = int(0);
  dummyGenerate = int(0);
}

DirichletModule::~DirichletModule() {
  delete cliParam;
}

void DirichletModule::Init(std::string & name) {
  cliParam->module_id = name;
  
}

long_seq* DirichletModule::readHyperparameters(bool blocking) {
  return &dummyHyperparameters;
}

float_seq* DirichletModule::readObservation(bool blocking) {
  return &dummyObservation;
}

int* DirichletModule::readDoTrain(bool blocking) {
  return &dummyDoTrain;
}

bool DirichletModule::writeClass(const long_seq &output) {
  return true;
}

int* DirichletModule::readGenerate(bool blocking) {
  return &dummyGenerate;
}

bool DirichletModule::writeCRP(const long_seq &output) {
  return true;
}

} // namespace

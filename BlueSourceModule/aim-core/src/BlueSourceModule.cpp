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

#include "BlueSourceModule.h"

using namespace rur;

BlueSourceModule::BlueSourceModule():
  cliParam(0)
{
  const char* const channel[11] = {"writeMicrophone", "writeBattery", "writeLight", "writeInfrared", "writeMotorCurrent1", "writeMotorCurrent2", "writeWheel1", "writeWheel2", "writeLed1", "writeLed2", "writeLed3"};
  cliParam = new Param();
}

BlueSourceModule::~BlueSourceModule() {
  delete cliParam;
}

void BlueSourceModule::Init(std::string & name) {
  cliParam->module_id = name;
}

bool BlueSourceModule::writeMicrophone(const int microphone) {
  return true;
}

bool BlueSourceModule::writeBattery(const int battery) {
  return true;
}

bool BlueSourceModule::writeLight(const int light) {
  return true;
}

bool BlueSourceModule::writeInfrared(const int infrared) {
  return true;
}

bool BlueSourceModule::writeMotorCurrent1(const int motorcurrent1) {
  return true;
}

bool BlueSourceModule::writeMotorCurrent2(const int motorcurrent2) {
  return true;
}

bool BlueSourceModule::writeWheel1(const int wheel1) {
  return true;
}

bool BlueSourceModule::writeWheel2(const int wheel2) {
  return true;
}

bool BlueSourceModule::writeLed1(const int led1) {
  return true;
}

bool BlueSourceModule::writeLed2(const int led2) {
  return true;
}

bool BlueSourceModule::writeLed3(const int led3) {
  return true;
}


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
 * @date                 Apr 18, 2014
 * @license              GNU General Lesser Public
 */

#include "RFXcomModule.h"

namespace rur {
using namespace yarp::os;

RFXcomModule::RFXcomModule():
  cliParam(0)
{
  const char* const channel[8] = {"readInput0", "readType0", "readSelect0", "writeOutput0", "readInput1", "readType1", "readSelect1", "writeOutput1"};
  cliParam = new Param();
  portInput0 = new BufferedPort<Bottle>();
  portType0 = new BufferedPort<Bottle>();
  portSelect0 = new BufferedPort<Bottle>();
  portOutput0 = new BufferedPort<Bottle>();
  portInput1 = new BufferedPort<Bottle>();
  portType1 = new BufferedPort<Bottle>();
  portSelect1 = new BufferedPort<Bottle>();
  portOutput1 = new BufferedPort<Bottle>();
}

RFXcomModule::~RFXcomModule() {
  delete cliParam;
  delete portInput0;
  delete portType0;
  delete portSelect0;
  delete portOutput0;
  delete portInput1;
  delete portType1;
  delete portSelect1;
  delete portOutput1;
}

void RFXcomModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/input0";
  portInput0->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/type0";
  portType0->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/select0";
  portSelect0->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/output0";
  portOutput0->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/input1";
  portInput1->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/type1";
  portType1->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/select1";
  portSelect1->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/output1";
  portOutput1->open(yarpPortName.str().c_str());
  
}

long_seq* RFXcomModule::readInput0(bool blocking) {
  Bottle *b = portInput0->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portInput0Buf.push_back(b->get(i).asInt());
    }
  }
  return &portInput0Buf;
}

int* RFXcomModule::readType0(bool blocking) {
  Bottle *b = portType0->read(blocking);
  if (b != NULL) { 
    portType0Buf = b->get(0).asInt();
    return &portType0Buf;
  }
  return NULL;
}

long_seq* RFXcomModule::readSelect0(bool blocking) {
  Bottle *b = portSelect0->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portSelect0Buf.push_back(b->get(i).asInt());
    }
  }
  return &portSelect0Buf;
}

bool RFXcomModule::writeOutput0(const int output0) {
  Bottle &output0Prepare = portOutput0->prepare();
  output0Prepare.clear();
  output0Prepare.addInt(output0);
  bool forceStrict = true; // wait till previous sends are complete
  portOutput0->write(forceStrict);
  return true;
}

long_seq* RFXcomModule::readInput1(bool blocking) {
  Bottle *b = portInput1->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portInput1Buf.push_back(b->get(i).asInt());
    }
  }
  return &portInput1Buf;
}

int* RFXcomModule::readType1(bool blocking) {
  Bottle *b = portType1->read(blocking);
  if (b != NULL) { 
    portType1Buf = b->get(0).asInt();
    return &portType1Buf;
  }
  return NULL;
}

long_seq* RFXcomModule::readSelect1(bool blocking) {
  Bottle *b = portSelect1->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portSelect1Buf.push_back(b->get(i).asInt());
    }
  }
  return &portSelect1Buf;
}

bool RFXcomModule::writeOutput1(const int output1) {
  Bottle &output1Prepare = portOutput1->prepare();
  output1Prepare.clear();
  output1Prepare.addInt(output1);
  bool forceStrict = true; // wait till previous sends are complete
  portOutput1->write(forceStrict);
  return true;
}

} // namespace

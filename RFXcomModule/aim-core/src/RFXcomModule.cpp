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
  const char* const channel[16] = {"readInput0", "readType0", "readSelect0", "writeOutput0", "readInput1", "readType1", "readSelect1", "writeOutput1", "readInput2", "readType2", "readSelect2", "writeOutput2", "readInput3", "readType3", "readSelect3", "writeOutput3"};
  cliParam = new Param();
  portInput0 = new BufferedPort<Bottle>();
  portType0 = new BufferedPort<Bottle>();
  portSelect0 = new BufferedPort<Bottle>();
  portOutput0 = new BufferedPort<Bottle>();
  portInput1 = new BufferedPort<Bottle>();
  portType1 = new BufferedPort<Bottle>();
  portSelect1 = new BufferedPort<Bottle>();
  portOutput1 = new BufferedPort<Bottle>();
  portInput2 = new BufferedPort<Bottle>();
  portType2 = new BufferedPort<Bottle>();
  portSelect2 = new BufferedPort<Bottle>();
  portOutput2 = new BufferedPort<Bottle>();
  portInput3 = new BufferedPort<Bottle>();
  portType3 = new BufferedPort<Bottle>();
  portSelect3 = new BufferedPort<Bottle>();
  portOutput3 = new BufferedPort<Bottle>();
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
  delete portInput2;
  delete portType2;
  delete portSelect2;
  delete portOutput2;
  delete portInput3;
  delete portType3;
  delete portSelect3;
  delete portOutput3;
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
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/input2";
  portInput2->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/type2";
  portType2->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/select2";
  portSelect2->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/output2";
  portOutput2->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/input3";
  portInput3->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/type3";
  portType3->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/select3";
  portSelect3->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/rfxcommodule" << name << "/output3";
  portOutput3->open(yarpPortName.str().c_str());
  
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

long_seq* RFXcomModule::readInput2(bool blocking) {
  Bottle *b = portInput2->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portInput2Buf.push_back(b->get(i).asInt());
    }
  }
  return &portInput2Buf;
}

int* RFXcomModule::readType2(bool blocking) {
  Bottle *b = portType2->read(blocking);
  if (b != NULL) { 
    portType2Buf = b->get(0).asInt();
    return &portType2Buf;
  }
  return NULL;
}

long_seq* RFXcomModule::readSelect2(bool blocking) {
  Bottle *b = portSelect2->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portSelect2Buf.push_back(b->get(i).asInt());
    }
  }
  return &portSelect2Buf;
}

bool RFXcomModule::writeOutput2(const int output2) {
  Bottle &output2Prepare = portOutput2->prepare();
  output2Prepare.clear();
  output2Prepare.addInt(output2);
  bool forceStrict = true; // wait till previous sends are complete
  portOutput2->write(forceStrict);
  return true;
}

long_seq* RFXcomModule::readInput3(bool blocking) {
  Bottle *b = portInput3->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portInput3Buf.push_back(b->get(i).asInt());
    }
  }
  return &portInput3Buf;
}

int* RFXcomModule::readType3(bool blocking) {
  Bottle *b = portType3->read(blocking);
  if (b != NULL) { 
    portType3Buf = b->get(0).asInt();
    return &portType3Buf;
  }
  return NULL;
}

long_seq* RFXcomModule::readSelect3(bool blocking) {
  Bottle *b = portSelect3->read(blocking);
  if (b != NULL) {
    for (int i = 0; i < b->size(); ++i) {
      portSelect3Buf.push_back(b->get(i).asInt());
    }
  }
  return &portSelect3Buf;
}

bool RFXcomModule::writeOutput3(const int output3) {
  Bottle &output3Prepare = portOutput3->prepare();
  output3Prepare.clear();
  output3Prepare.addInt(output3);
  bool forceStrict = true; // wait till previous sends are complete
  portOutput3->write(forceStrict);
  return true;
}

} // namespace

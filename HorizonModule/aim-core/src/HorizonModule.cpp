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
 * @date                 Oct 28, 2013
 * @license              GNU General Lesser Public
 */

#include "HorizonModule.h"

namespace rur {
using namespace yarp::os;

HorizonModule::HorizonModule():
  cliParam(0)
{
  const char* const channel[3] = {"readChannel", "readCommand", "readAddress"};
  cliParam = new Param();
  portChannel = new BufferedPort<Bottle>();
  portCommand = new BufferedPort<Bottle>();
  portAddress = new BufferedPort<Bottle>();
}

HorizonModule::~HorizonModule() {
  delete cliParam;
  delete portChannel;
  delete portCommand;
  delete portAddress;
}

void HorizonModule::Init(std::string & name) {
  cliParam->module_id = name;
  
  std::stringstream yarpPortName;
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/horizonmodule" << name << "/channel";
  portChannel->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/horizonmodule" << name << "/command";
  portCommand->open(yarpPortName.str().c_str());
  
  yarpPortName.str(""); yarpPortName.clear();
  yarpPortName << "/horizonmodule" << name << "/address";
  portAddress->open(yarpPortName.str().c_str());
  
}

int* HorizonModule::readChannel(bool blocking) {
  Bottle *b = portChannel->read(blocking);
  if (b != NULL) { 
    portChannelBuf = b->get(0).asInt();
    return &portChannelBuf;
  }
  return NULL;
}

std::string* HorizonModule::readCommand(bool blocking) {
  Bottle *b = portCommand->read(blocking);
  if (b != NULL) { 
    portCommandBuf = b->get(0).asString();
    return &portCommandBuf;
  }
  return NULL;
}

std::string* HorizonModule::readAddress(bool blocking) {
  Bottle *b = portAddress->read(blocking);
  if (b != NULL) { 
    portAddressBuf = b->get(0).asString();
    return &portAddressBuf;
  }
  return NULL;
}

} // namespace

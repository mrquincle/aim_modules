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

#ifndef HORIZONMODULE_H_
#define HORIZONMODULE_H_

#include <string>
#include <vector>
#include <string>
#include <vector>
#include <sstream>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>

namespace rur {

struct Param {
  std::string module_id;
  int parameter;
};

typedef std::vector<int> long_seq;

class HorizonModule {
private:
  Param *cliParam;
  
  yarp::os::Network yarp;
  int portChannelBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portChannel;
  std::string portCommandBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portCommand;
protected:
  static const int channel_count = 2;
  const char* channel[2];
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readChannel(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  std::string *readCommand(bool blocking=false);
  
public:
  // Default constructor
  HorizonModule();
  
  // Default destructor
  virtual ~HorizonModule();
  
  // Extend this with your own code, first call HorizonModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
};
} // End of namespace

#endif // HORIZONMODULE_H_

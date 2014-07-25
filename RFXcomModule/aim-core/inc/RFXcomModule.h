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

#ifndef RFXCOMMODULE_H_
#define RFXCOMMODULE_H_

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

class RFXcomModule {
private:
  Param *cliParam;
  
  yarp::os::Network yarp;
  long_seq portInput0Buf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portInput0;
  int portType0Buf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portType0;
  long_seq portSelect0Buf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portSelect0;
  yarp::os::BufferedPort<yarp::os::Bottle> *portOutput0;
  long_seq portInput1Buf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portInput1;
  int portType1Buf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portType1;
  long_seq portSelect1Buf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portSelect1;
  yarp::os::BufferedPort<yarp::os::Bottle> *portOutput1;
protected:
  static const int channel_count = 8;
  const char* channel[8];
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readInput0(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readType0(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readSelect0(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeOutput0(const int output0);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readInput1(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readType1(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readSelect1(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeOutput1(const int output1);
  
public:
  // Default constructor
  RFXcomModule();
  
  // Default destructor
  virtual ~RFXcomModule();
  
  // Extend this with your own code, first call RFXcomModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
};
} // End of namespace

#endif // RFXCOMMODULE_H_

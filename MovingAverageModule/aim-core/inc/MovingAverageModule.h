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

#ifndef MOVINGAVERAGEMODULE_H_
#define MOVINGAVERAGEMODULE_H_

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

class MovingAverageModule {
private:
  Param *cliParam;
  
  yarp::os::Network yarp;
  int portInputValue;
  yarp::os::BufferedPort<yarp::os::Bottle> *portInput;
  yarp::os::BufferedPort<yarp::os::Bottle> *portAverage;
protected:
  static const int channel_count = 2;
  const char* channel[2];
public:
  MovingAverageModule();
  
  ~MovingAverageModule();
  
  // Extend this with your own code, first call MovingAverageModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() {}
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readInput(bool blocking=false);
  // Write to this function and assume it ends up at some receiving module
  bool writeAverage(const int output);
};
} // End of namespace

#endif // MOVINGAVERAGEMODULE_H_

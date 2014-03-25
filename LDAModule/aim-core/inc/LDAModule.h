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
 * @date                 Mar 24, 2014
 * @license              GNU General Lesser Public
 */

#ifndef LDAMODULE_H_
#define LDAMODULE_H_

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

class LDAModule {
private:
  Param *cliParam;
  
  yarp::os::Network yarp;
  long_seq portSampleBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portSample;
  int portModeBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portMode;
  int portLagBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portLag;
  double portConvergenceBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portConvergence;
  yarp::os::BufferedPort<yarp::os::Bottle> *portLeftWheel;
protected:
  static const int channel_count = 5;
  const char* channel[5];
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readSample(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readMode(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readLag(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  double *readConvergence(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeLeftWheel(const int output);
  
public:
  // Default constructor
  LDAModule();
  
  // Default destructor
  virtual ~LDAModule();
  
  // Extend this with your own code, first call LDAModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
};
} // End of namespace

#endif // LDAMODULE_H_

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
 * @date                 Nov 18, 2013
 * @license              GNU General Lesser Public
 */

#ifndef CLUSTERMODULE_H_
#define CLUSTERMODULE_H_

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

class ClusterModule {
private:
  Param *cliParam;
  
  yarp::os::Network yarp;
  long_seq portTrainBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portTrain;
  long_seq portTestBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portTest;
  yarp::os::BufferedPort<yarp::os::Bottle> *portClass;
  int portClusterCountBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portClusterCount;
  int portMethodBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portMethod;
protected:
  static const int channel_count = 5;
  const char* channel[5];
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readTrain(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readTest(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeClass(const int output);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readClusterCount(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readMethod(bool blocking=false);
  
public:
  // Default constructor
  ClusterModule();
  
  // Default destructor
  virtual ~ClusterModule();
  
  // Extend this with your own code, first call ClusterModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
};
} // End of namespace

#endif // CLUSTERMODULE_H_

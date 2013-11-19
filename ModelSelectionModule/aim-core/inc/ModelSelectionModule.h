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
 * @date                 Oct 18, 2013
 * @license              GNU General Lesser Public
 */

#ifndef MODELSELECTIONMODULE_H_
#define MODELSELECTIONMODULE_H_

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

class ModelSelectionModule {
private:
  Param *cliParam;
  
  yarp::os::Network yarp;
  long_seq portResidualsBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portResiduals;
  int portModelParameterCountBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portModelParameterCount;
  int portMethodBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portMethod;
  yarp::os::BufferedPort<yarp::os::Bottle> *portModelQuality;
protected:
  static const int channel_count = 4;
  const char* channel[4];
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readResiduals(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readModelParameterCount(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readMethod(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeModelQuality(const float output);
  
public:
  // Default constructor
  ModelSelectionModule();
  
  // Default destructor
  virtual ~ModelSelectionModule();
  
  // Extend this with your own code, first call ModelSelectionModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
};
} // End of namespace

#endif // MODELSELECTIONMODULE_H_

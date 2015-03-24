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
 * @author               Anne van Rossum
 * @copyright            Distributed Organisms B.V. (DoBots)
 * @date                 Mar 23, 2015
 * @license              LGPLv3
 */

#ifndef RECOMMENDERMODULE_H_
#define RECOMMENDERMODULE_H_

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

class RecommenderModule {
private:
  Param *cliParam;
  
  yarp::os::Network yarp;
  std::string portDocumentBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portDocument;
  std::string portTermBuf;
  yarp::os::BufferedPort<yarp::os::Bottle> *portTerm;
  yarp::os::BufferedPort<yarp::os::Bottle> *portRecommendation;
protected:
  static const int channel_count = 3;
  const char* channel[3];
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  std::string *readDocument(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  std::string *readTerm(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeRecommendation(const std::string output);
  
public:
  // Default constructor
  RecommenderModule();
  
  // Default destructor
  virtual ~RecommenderModule();
  
  // Extend this with your own code, first call RecommenderModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
};
} // End of namespace

#endif // RECOMMENDERMODULE_H_

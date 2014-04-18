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

namespace rur {

struct Param {
  std::string module_id;
  int parameter;
};

typedef std::vector<int> long_seq;

class RFXcomModule {
private:
  Param *cliParam;
  
  long_seq dummyAudio;
  int dummyInfrared;
protected:
  static const int channel_count = 3;
  const char* channel[3];
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
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readAudio(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readInfrared(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeLeftWheel(const int output);
  
};
} // End of namespace

#endif // RFXCOMMODULE_H_

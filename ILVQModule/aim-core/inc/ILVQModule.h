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
 * @date                 Oct 29, 2013
 * @license              GNU General Lesser Public
 */

#ifndef ILVQMODULE_H_
#define ILVQMODULE_H_

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

class ILVQModule {
private:
  Param *cliParam;
  
  long_seq dummyTraining;
  long_seq dummyTesting;
protected:
  static const int channel_count = 3;
  const char* channel[3];
public:
  // Default constructor
  ILVQModule();
  
  // Default destructor
  virtual ~ILVQModule();
  
  // Extend this with your own code, first call ILVQModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readTraining(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readTesting(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeResult(const int value);
  
};
} // End of namespace

#endif // ILVQMODULE_H_

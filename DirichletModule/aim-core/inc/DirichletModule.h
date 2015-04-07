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
 * @date                 Mar 31, 2015
 * @license              GNU General Lesser Public
 */

#ifndef DIRICHLETMODULE_H_
#define DIRICHLETMODULE_H_

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

typedef std::vector<float> float_seq;

typedef std::vector<double> double_seq;

class DirichletModule {
private:
  Param *cliParam;
  
  long_seq dummyHyperparameters;
  float_seq dummyObservation;
  int dummyDoTrain;
  int dummyGenerate;
protected:
  static const int channel_count = 6;
  const char* channel[6];
public:
  // Default constructor
  DirichletModule();
  
  // Default destructor
  virtual ~DirichletModule();
  
  // Extend this with your own code, first call DirichletModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  long_seq *readHyperparameters(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: caller is responsible for evoking vector->clear()
  float_seq *readObservation(bool blocking=false);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readDoTrain(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeClass(const long_seq &output);
  
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  int *readGenerate(bool blocking=false);
  
  // Write to this function and assume it ends up at some receiving module
  bool writeCRP(const long_seq &output);
  
};
} // End of namespace

#endif // DIRICHLETMODULE_H_

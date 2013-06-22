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

#ifndef BLUESOURCEMODULE_H_
#define BLUESOURCEMODULE_H_

#include <string>
#include <vector>
#include <string>
#include <vector>

namespace rur {

struct Param {
  std::string module_id;
  int sensor_id;
};

class BlueSourceModule {
private:
  Param *cliParam;
  
protected:
  static const int channel_count = 11;
  const char* channel[11];
public:
  BlueSourceModule();
  
  ~BlueSourceModule();
  
  // Extend this with your own code, first call BlueSourceModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() {}
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  // Write to this function and assume it ends up at some receiving module
  bool writeMicrophone(const int microphone);
  // Write to this function and assume it ends up at some receiving module
  bool writeBattery(const int battery);
  // Write to this function and assume it ends up at some receiving module
  bool writeLight(const int light);
  // Write to this function and assume it ends up at some receiving module
  bool writeInfrared(const int infrared);
  // Write to this function and assume it ends up at some receiving module
  bool writeMotorCurrent1(const int motorcurrent1);
  // Write to this function and assume it ends up at some receiving module
  bool writeMotorCurrent2(const int motorcurrent2);
  // Write to this function and assume it ends up at some receiving module
  bool writeWheel1(const int wheel1);
  // Write to this function and assume it ends up at some receiving module
  bool writeWheel2(const int wheel2);
  // Write to this function and assume it ends up at some receiving module
  bool writeLed1(const int led1);
  // Write to this function and assume it ends up at some receiving module
  bool writeLed2(const int led2);
  // Write to this function and assume it ends up at some receiving module
  bool writeLed3(const int led3);
};
} // End of namespace

#endif // BLUESOURCEMODULE_H_

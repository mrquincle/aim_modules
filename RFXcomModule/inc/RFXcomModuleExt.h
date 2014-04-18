/**
 * @file RFXcomModuleExt.h
 * @brief RFXcomModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2014 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Apr 18, 2014
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <RFXcomModule.h>

namespace rur {

/**
 * Your Description of this module.
 */
class RFXcomModuleExt: public RFXcomModule {
public:
	//! The constructor
	RFXcomModuleExt();

	//! The destructor
	virtual ~RFXcomModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the RFXcomModuleMain will stop the module
	bool Stop();

	void Init(std::string&param);
private:
	int device;
};

}


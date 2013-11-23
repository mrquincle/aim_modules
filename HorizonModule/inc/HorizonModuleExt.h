/**
 * @file HorizonModuleExt.h
 * @brief HorizonModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2013 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Oct 28, 2013
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <HorizonModule.h>

#include <remote.hpp>
#include <algorithm>

namespace rur {

/**
 * Your Description of this module.
 */
class HorizonModuleExt: public HorizonModule {
public:
	//! The constructor
	HorizonModuleExt();

	//! The destructor
	virtual ~HorizonModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the HorizonModuleMain will stop the module
	bool Stop();

	inline void getDigits(int value, std::vector<int> & digits) {
		while (value) {
			digits.push_back(value % 10);
			value /= 10;
		}
		std::reverse(digits.begin(), digits.end());
	}
	
	void SetController(std::string address);
private:
	//! Remote controller for box
	horizonremote::RemoteController *controller;
};

}


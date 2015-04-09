/**
 * @file RotateModuleExt.h
 * @brief RotateModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Crownstone". 
 * This software is published under the GNU Lesser General Public License license (LGPLv3).
 *
 * Copyright © 2015 Anne van Rossum <anne@dobots.nl>
 *
 * @author                   Anne van Rossum
 * @date                     Apr  9, 2015
 * @organisation             Distributed Organisms B.V.
 * @project                  Crownstone
 */

#include <RotateModule.h>

namespace rur {

/**
 * Your Description of this module.
 */
class RotateModuleExt: public RotateModule {
public:
	//! The constructor
	RotateModuleExt();

	//! The destructor
	virtual ~RotateModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the RotateModuleMain will stop the module
	bool Stop();
};

}


/**
 * @file ILVQModuleExt.h
 * @brief ILVQModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2013 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Oct 29, 2013
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <ILVQModule.h>

#include <ILVQ_XSZ.h>

namespace rur {

/**
 * Your Description of this module.
 */
class ILVQModuleExt: public ILVQModule {
public:
	//! The constructor
	ILVQModuleExt();

	//! The destructor
	virtual ~ILVQModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the ILVQModuleMain will stop the module
	bool Stop();
private:
	dobots::ILVQ_XSZ *ilvq;

	int sample_dimension;
};

}


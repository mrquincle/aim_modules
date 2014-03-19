/**
 * @file DatasetModuleExt.h
 * @brief DatasetModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2014 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Mar 19, 2014
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <DatasetModule.h>

namespace rur {

/**
 * Your Description of this module.
 */
class DatasetModuleExt: public DatasetModule {
public:
	//! The constructor
	DatasetModuleExt();

	//! The destructor
	virtual ~DatasetModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the DatasetModuleMain will stop the module
	bool Stop();

	//! First. Tick() will call Train() for a few times
	void Train();
	
	//! Second. Tick() will call Test() once
	void Test();

	//! Third. CheckResult is called, now Test() can be called again if you'd like...
	void CheckResult();
private:
	int test_index, train_index;
};

}


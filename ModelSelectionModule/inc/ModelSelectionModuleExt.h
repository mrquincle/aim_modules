/**
 * @file ModelSelectionModuleExt.h
 * @brief ModelSelectionModule extension
 *
 * This file is created at "Your Company". It is open-source software and part of "Specific Software Project". This 
 * software is published under the GNU Lesser General Public license (LGPL).
 *
 * Copyright © 2013 Your Name <your@email>
 *
 * @author	Your Name
 * @date	Current date
 * @company	Your Company
 * @project	Specific Software Project
 */

#include <ModelSelectionModule.h>

namespace rur {

/**
 * Your Description of this module.
 */
class ModelSelectionModuleExt: public ModelSelectionModule {
public:
	//! The constructor
	ModelSelectionModuleExt();

	//! The destructor
	virtual ~ModelSelectionModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the ModelSelectionModuleMain will stop the module
	bool Stop();
};

}


/**
 * @file RecommenderModuleExt.h
 * @brief RecommenderModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "COMMIT P3". 
 * This software is published under the LGPLv3 license.
 *
 * Copyright © 2015 Anne van Rossum <anne@dobots.nl>
 *
 * @author                   Anne van Rossum
 * @date                     Mar 23, 2015
 * @organisation             Distributed Organisms B.V. (DoBots)
 * @project                  COMMIT P3 / Sensei
 */
#include <RecommenderModule.h>

#include <Library.h>

namespace rur {

struct Result {
	std::string docId;
	std::string term;
	std::string probability;
};

/**
 * Your Description of this module.
 */
class RecommenderModuleExt: public RecommenderModule {
public:
	//! The constructor
	RecommenderModuleExt();

	//! The destructor
	virtual ~RecommenderModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the RecommenderModuleMain will stop the module
	bool Stop();
private:
	//! Library to store documents
	Library library;
	//! Results
	std::vector<Result*> results;
};

}


/**
 * @file ModelSelectionModuleExt.h
 * @brief ModelSelectionModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2013 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Oct 18, 2013
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <ModelSelectionModule.h>

namespace rur {

enum QualityCriterion { AIC, BIC, QC_COUNT };

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

	//! Calculate model quality
	float ModelQuality(QualityCriterion criterion);

	//! Set method
	inline void setMethod(const int m) {
		if (m < 0 || m >= QC_COUNT) return;
		criterion = (QualityCriterion)m;
	}
private:
	// RSS is the Residual Sum of Squares
	int RSS;

	// the number of model parameters
	int k;

	// the number of data elements
	int n;

	// criterion to define model quality
	QualityCriterion criterion;
};

}


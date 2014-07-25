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
#include <rfxcom.h>

namespace rur {

// might look as a smart move, and READ_TYPE(0) will work, but READ_TYPE(d) will not...
#define READ_TYPE(dev) readType ## dev()
#define READ_SELECT(dev) readSelect ## dev()
#define READ_INPUT(dev) readInput ## dev()

#define TWO_DEVICES

struct RFXcomDevice {
	uint8_t type;
	uint8_t id[3];
	uint8_t channel; // outgoing channel in middleware
	bool init;
};

/**
 * The RFXcom module sends and receives messages from the RFXcom devices attached to an USB port.
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

	void SetMode(RFXmsg & msg);

//	void TurnOnSwitch(RFXmsg &msg);
	RFXcomDevice * GetDevice(int type, long_seq & id);

	bool SendCommand(RFXmsg &msg, RFXcomDevice &device, long_seq &command);

	void ReceiveAll();

	void Receive(RFXmsg & msg);

	void Send(RFXmsg & msg);
	
	void Clear(RFXmsg & msg);
	
	void Display(RFXmsg & msg);
protected:
	int* readType(int channel) {
		switch(channel) {
			case 0: return READ_TYPE(0);
			case 1: return READ_TYPE(1);
#ifndef TWO_DEVICES
			case 2: return READ_TYPE(2);
			case 3: return READ_TYPE(3);
#endif
		}
	}
	long_seq* readSelect(int channel) {
		switch(channel) {
			case 0: return READ_SELECT(0);
			case 1: return READ_SELECT(1);
#ifndef TWO_DEVICES
			case 2: return READ_SELECT(2);
			case 3: return READ_SELECT(3);
#endif
		}
	}
	long_seq* readInput(int channel) {
		switch(channel) {
			case 0: return READ_INPUT(0);
			case 1: return READ_INPUT(1);
#ifndef TWO_DEVICES
			case 2: return READ_INPUT(2);
			case 3: return READ_INPUT(3);
#endif
		}
	}
	bool writeOutput(int channel, const int output) {
		switch(channel) {
			case 0: return writeOutput0(output);
			case 1: return writeOutput1(output);
#ifndef TWO_DEVICES
			case 2: return writeOutput2(output);
			case 3: return writeOutput3(output);
#endif
		}
	}
	
private:
	int fd;

	uint8_t seq_cntr;

	bool stop = false;

	std::vector<RFXcomDevice*> devices;
};

}


/**
 * @file RFXcomModuleExt.cpp
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

#include <RFXcomModuleExt.h>

#include <iostream>
#include <sstream>
#include <cstdio>       // standard input / output functions
#include <cstdlib>
#include <cstring>       // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <iomanip>

using namespace rur;

//! Replace with your own code
RFXcomModuleExt::RFXcomModuleExt() {
	for (int d = 0; d < NOF_DEVICES; ++d) {
		RFXcomDevice *dev = new RFXcomDevice();
		dev->init.type = false;
		dev->init.id = false;
		dev->init.channel = false;
		devices.push_back(dev);
	}
	//Init();	
}

//! Replace with your own code
RFXcomModuleExt::~RFXcomModuleExt() {

}

void RFXcomModuleExt::Init(std::string& param) {
	RFXcomModule::Init(param);

	std::cout << "Open device /dev/rfxcom" << std::endl;
	fd = open( "/dev/rfxcom", O_RDWR| O_NOCTTY );
	if (fd < 0) {
		std::cerr << "Could not open /dev/rfxcom" << std::endl;
		return;
	}
	std::cout << "Nice, rfxcom device opened" << std::endl;
	struct termios tty;
	struct termios tty_old;
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( fd, &tty ) != 0 ) {
		std::cerr << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
		return;
	} else {
		std::cout << "The rfxcom device is successfully opened" << std::endl;
	}
	

	/* Save old tty parameters */
	tty_old = tty;

	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B38400);
	cfsetispeed (&tty, (speed_t)B38400);

	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;        // Make 8n1, set parity bit off
	tty.c_cflag     &=  ~CSTOPB;        // clear stop bit
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;            // 8-bit

	tty.c_cflag     &=  ~CRTSCTS;       // no flow control
	tty.c_cc[VMIN]   =   1;             // read doesn't block
	tty.c_cc[VTIME]  =   5;             // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL; // turn on READ & ignore ctrl lines

	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( fd, TCIFLUSH );
	if ( tcsetattr ( fd, TCSANOW, &tty ) != 0) {
		std::cerr << "Error " << errno << " from tcsetattr" << std::endl;
		return;
	}

	std::cout << "Send reset command to rfcom device" << std::endl;
	RFXmsg msg;
	seq_cntr = 0;
	
	Clear(msg);
	Send(msg);
	Display(msg);

	// wait one second
	sleep(1);

	msg.control.message_type = MSG_CONTROL;
	msg.control.sub_type = MSG_CONTROL_MODE;
	msg.control.seqn = ++seq_cntr;
	msg.control.cmd = MSG_CONTROL_CMD_GET_STATUS;
	Send(msg);
	Display(msg);

	Receive(msg);
	Display(msg);

	//sleep(2);
	//Clear(msg);
	//SetMode(msg);
	//Display(msg);
	//Receive(msg);
	//Display(msg);
	
//	Clear(msg);
//	TurnOnSwitch(msg);
//	Display(msg);
//	Receive(msg);
//	Display(msg);
	std::cout << "Done with initialization" << std::endl;
}

void RFXcomModuleExt::Clear(RFXmsg & msg) {
	msg.control.message_length = 14-1;
	for (int i = 1; i < msg.control.message_length+1; i++) {
		msg.raw[i] = 0;
	}
}

void RFXcomModuleExt::SetMode(RFXmsg & msg) {
	std::cout << "Set mode" << std::endl;
	msg.control.message_length = 14-1; // length minus 1
	msg.control.message_type = MSG_CONTROL;
	msg.control.sub_type = MSG_CONTROL_MODE;
	msg.control.cmd = MSG_CONTROL_CMD_SET_MODE; 
	msg.control.payload[0] = DEVICE_RFX_TRX_433;
 	std::bitset<8> msg3;
	msg3[MODE_RSL] = 1;
	msg.control.payload[2] = (uint8_t)msg3.to_ulong();
 	std::bitset<8> msg4;
	//msg4[MODE_BLINDST0] = 1;
	msg4[MODE_LA_CROSSE] = 1;
	msg4[MODE_HIDEKI] = 1;
	msg4[MODE_AD] = 1;
	msg.control.payload[3] = (uint8_t)msg4.to_ulong();
 	std::bitset<8> msg5;
	msg5[MODE_ATI] = 1;
	msg5[MODE_OREGON_SCIENTIFIC] = 1;
	msg5[MODE_HOMEEASY_EU] = 1;
	msg5[MODE_AC] = 1;
	msg5[MODE_ARC] = 1;
	msg5[MODE_X10] = 1;
	msg.control.payload[4] = (uint8_t)msg5.to_ulong();

	Send(msg);
}

bool RFXcomModuleExt::SendCommand(RFXmsg &msg, RFXcomDevice &device, long_seq &command) {
	std::cout << "Send command" << std::endl;

	if (!device.init.type || !device.init.channel) {
		std::cerr << "It doesn't seem the device type has been communicated" << std::endl;
		std::cerr << "Have you forgotten to send this info to the /type channel?" << std::endl;
		std::cerr << "Or is there a synchronicity issue?" << std::endl;
		RFXcomBufferItem *item = new RFXcomBufferItem(command);
		device.buffer.push(item);
		return false;
	}

	switch(device.type) {
		case MSG_LIGHTING1: {
			// this is a simple switch 
			msg.lighting1.message_length = 8-1;
			msg.lighting1.message_type = MSG_LIGHTING1;
			msg.lighting1.sub_type = MSG_LIGHTING1_RISING_SUN; // CONRAD_RSL_366R

			msg.lighting1.housecode = 0x41;
			msg.lighting1.unitcode = device.id[0];
			msg.lighting1.cmd = command[0];
			msg.lighting1.filler = 0;
			msg.lighting1.rssi = 0;
			break;

		}
		case MSG_LIGHTING5: {
			// depends on switch, I thought this was the Conrad, but that one is MSG_LIGHTING1
			msg.lighting5.message_length = 11-1;
			msg.lighting5.message_type = MSG_LIGHTING5;
			// currently we just assume a conrad switch
			msg.lighting5.sub_type = MSG_LIGHTING5_CONRAD_RSL2;

			// remote/switch/unit ID
			// for subtype 0x04: The unit ID can be 0x00 00 01 to 0xFF FF FF
			// for subtype 0x04: 0x01 to 0x10 = unit 1 to 16
			msg.lighting5.id[0] = device.id[0];
			msg.lighting5.id[1] = device.id[1];
			msg.lighting5.id[2] = device.id[2];
			// currently we just pick unitcode 1
			msg.lighting5.unitcode = 1;
			msg.lighting5.cmd = command[0];
			msg.lighting5.level = 0; // only used for LIGHTWAVE_RF / SIEMENS
			msg.lighting5.rssi = 0; // for transmitter 0
			break;
		}
		default: {
			std::cerr << "Error, device.type \"" << device.type << "\" is not known" << std::endl;
			std::cerr << "Currently supported: \"" << MSG_LIGHTING1 << "\" and \"" << MSG_LIGHTING5 << "\"" << std::endl;
			return false;
		}
	}
			
	Send(msg);
	return true;
}


void RFXcomModuleExt::Send(RFXmsg &msg) {
//	if (msg.control.message_length != 13) {
//		std::cerr << "The RFXmsg message_length field should be 13" << std::endl;
//		return;
//	}
	msg.control.seqn = ++seq_cntr;
	int n_written = write( fd, msg.raw, msg.control.message_length+1);
	if (n_written != msg.control.message_length+1) {
		std::cerr << "Error, could not write " << msg.control.message_length+1 << " bytes, but only " << n_written << std::endl;
	}
}

void RFXcomModuleExt::Display(RFXmsg &msg) {
	std::cout << "Message: ";
	for (int i = 0; i < msg.control.message_length+1; i++) {
		std::cout << std::internal << std::setfill('0') << std::hex << std::setw(2) << (int)msg.raw[i] << " ";
	}
	std::cout << std::endl;
}

void RFXcomModuleExt::ReceiveAll() {
	RFXmsg msg;
	Receive(msg);

	uint8_t type = msg.standard.message_type;
	switch(type) {
		case MSG_LIGHTING2: {
			long_seq id;
			id.push_back(msg.lighting2.unitcode);
			RFXcomDevice *dev = GetDevice(type, id);
			if (dev == NULL) {
				std::cerr << "Could not find port for device (make silent later)" << std::endl;
				return;
			} 
			int channel = dev->channel;
			int value = msg.lighting2.level;
			writeOutput(channel, value); // 0x00 if no movement anymore, 0xFF when movement detected
			break;
		}
		default: {
			std::cerr << "Don't know this protocol yet" << std::endl;
		}
	}
}

RFXcomDevice * RFXcomModuleExt::GetDevice(int type, long_seq & id) {
	for (int i = 0; i < devices.size(); i++) {
		if (devices[i]->type == type) {
			std::cout << "For now, success! We do not check on id yet" << std::endl;
			std::cout << "however, it is ";
			for (int j = 0; j < 3; j++) {
				std::cout << devices[i]->id[j] << ' ';
			}
			std::cout << " vs ";
			for (int j = 0; j < id.size(); j++) {
				std::cout << id[j] << ' ';
			}
			std::cout << std::endl;
			return devices[i];
		}
	}
	return NULL;
}

void RFXcomModuleExt::Receive(RFXmsg &msg) {
	int n = 0;
	std::cout << "Read response from rfxcom device" << std::endl;
	char buf [1];
	memset (&buf, '\0', sizeof buf);
	int i = 0, stop = 255;
	std::ostringstream resp;
	do {
		n = read( fd, &buf, 1 );
		if (i == 0) stop = buf[0];
		msg.raw[i] = buf[0];
	} while( n > 0 && i++ < stop && stop);

	if (n < 0) {
		std::cout << "Error reading: " << strerror(errno) << std::endl;
	} else if (n == 0) {
		std::cout << "Read nothing!" << std::endl;
	} 
}

/**
 * Read all the ports corresponding to all possible devices.
 */
void RFXcomModuleExt::Tick() {
	int *type; long_seq *id = NULL, *cmd = NULL;

	for (int d = 0; d < devices.size(); ++d) {
		if (!devices[d]) {
			std::cerr << "Device " << d << " seems to be null!" << std::endl;
			continue;
		}
		type = readType(d);
		if (type) {
			devices[d]->type = *type;
			devices[d]->init.type = true;
			std::cout << "Device type " << 
				//std::dec <<
				std::internal << std::setfill('0') << std::hex << std::setw(2) <<
				"0x" << (int)devices[d]->type << " set for channel " << d << std::endl;
			devices[d]->channel = d;
		}
		id = readSelect(d);
		if (id && !id->empty()) {
			std::cout << "Device id ";
			for (int i = 0; i < id->size() && i < 3; i++) {
				devices[d]->id[i] = (*id)[i];
				std::cout << (int)devices[d]->id[i] << ' ';
			}
			std::cout << " set for channel " << d << std::endl;
			devices[d]->init.channel = true;
			id->clear();
		}	
		cmd = readInput(d);
		if (cmd && !cmd->empty()) {
			RFXmsg msg;
			Clear(msg);
			std::cout << "Send command to device from channel " << d << std::endl;
			bool success = SendCommand(msg,*devices[d],*cmd);
			if (success) {
				Display(msg);
				Receive(msg);
				Display(msg);
			}
			cmd->clear();
		}
		if (!devices[d]->buffer.empty()) {
			if (devices[d]->init.type && devices[d]->init.channel) {
				// we have a message in the buffer and are now properly initialized, resend that message
				RFXmsg msg;
				Clear(msg);
				std::cout << "Resend command to device from channel " << d << std::endl;
				RFXcomBufferItem *item = devices[d]->buffer.front();
				if (!item) {
					std::cerr << "Resend failed. Item is null!" << std::endl;
					devices[d]->buffer.pop();
					continue;
				}
				bool success = SendCommand(msg,*devices[d],*cmd);
				if (success) {
					Display(msg);
					Receive(msg);
					Display(msg);
				}
				devices[d]->buffer.pop();
			}
		}
	}

#ifdef TEST
	RFXmsg msg;
	Clear(msg);
	Receive(msg);
	Display(msg);	
#endif

	//ReceiveAll(); blocks

	//sleep(1);
}

//! Replace with your own code
bool RFXcomModuleExt::Stop() {
	return stop;
}


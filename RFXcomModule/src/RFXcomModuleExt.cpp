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
	//Init();	
}

//! Replace with your own code
RFXcomModuleExt::~RFXcomModuleExt() {

}

void RFXcomModuleExt::Init(std::string& param) {
	std::cout << "Open device /dev/rfxcom" << std::endl;
	int USB = open( "/dev/rfxcom", O_RDWR| O_NOCTTY );
	std::cout << "Nice, rfxcom device opened" << std::endl;
	struct termios tty;
	struct termios tty_old;
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 ) {
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
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
		std::cerr << "Error " << errno << " from tcsetattr" << std::endl;
		return;
	}

	std::cout << "Send reset command to rfcom device" << std::endl;
	unsigned char cmd[14]; 
	for (int i = 0; i < 14; i++) {
		cmd[i] = 0;
	}
	cmd[0] = 13;
	std::cout << "In specific: ";
	for (int i = 0; i < 14; i++) {
		//std::cout << std::showbase << std::internal << std::setfill('0') << std::hex << std::setw(4) << (int)cmd[i] << " ";
		std::cout << std::internal << std::setfill('0') << std::hex << std::setw(2) << (int)cmd[i] << " ";
	}
	std::cout << std::endl;
	int n_written = write( USB, cmd, sizeof(cmd));
	if (n_written != 14) {
		std::cerr << "Error, could not write 14 bytes, but only " << n_written << std::endl;
	}
	// wait one second
	sleep(1);
	std::cout << "Send command to rfcom device" << std::endl;
	for (int i = 0; i < 14; i++) {
		switch(i) {
			case 0: cmd[i] = 13;
				break;
			case 3: cmd[i] = 1;
				break;
			case 4: cmd[i] = 2;
				break;
			default: cmd[i] = 0;
		}
	}
	n_written = write( USB, cmd, sizeof(cmd));
	if (n_written != 14) {
		std::cerr << "Error, could not write 14 bytes, but only " << n_written << std::endl;
	}
	std::cout << "In specific: ";
	for (int i = 0; i < 14; i++) {
		//std::cout << std::showbase << std::internal << std::setfill('0') << std::hex << std::setw(4) << (int)cmd[i] << " ";
		std::cout << std::internal << std::setfill('0') << std::hex << std::setw(2) << (int)cmd[i] << " ";
	}
	std::cout << std::endl;

	int n = 0;
	std::string response;

	std::cout << "Read response from rfcom device" << std::endl;
	char buf [1];
	memset (&buf, '\0', sizeof buf);
	int i = 0, stop = 255;
	std::ostringstream resp;
	do {
		n = read( USB, &buf, 1 );
		if (i == 0) {
		       stop = buf[0];
		}
		resp << std::internal << std::setfill('0') << std::hex << std::setw(2) << (int)buf[0] << ' ';
		//response = std::to_string((int)buf[0]);
		//std::cout << (int)buf[0] << ' ' << std::endl;
	} while( n > 0 && i++ < stop);
//	std::cout << std::endl;
	response = resp.str();

	if (n < 0) {
		std::cout << "Error reading: " << strerror(errno) << std::endl;
	}
	else if (n == 0) {
		std::cout << "Read nothing!" << std::endl;
	}
	else {
		std::cout << "Response (status): " << response << std::endl;
	}
	std::cout << "Ready!" << std::endl;
}

//! Replace with your own code
void RFXcomModuleExt::Tick() {
}

//! Replace with your own code
bool RFXcomModuleExt::Stop() {
	return false;
}


/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief ...
 * @file keys.cpp
 * 
 * This file is created at Almende B.V. and Distributed Organisms B.V. It is open-source software and belongs to a
 * larger suite of software that is meant for research on self-organization principles and multi-agent systems where
 * learning algorithms are an important aspect.
 *
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless, we personally strongly object
 * against this software being used for military purposes, factory farming, animal experimentation, and "Universal
 * Declaration of Human Rights" violations.
 *
 * Copyright (c) 2013 Anne C. van Rossum <anne@almende.org>
 *
 * @author    Anne C. van Rossum
 * @date      Oct 28, 2013
 * @project   Replicator 
 * @company   Almende B.V.
 * @company   Distributed Organisms B.V.
 * @case      Clustering
 */

#include <keys.hpp>

const int getKey(const std::string name) {
	if (name == STR_POWER) return KEY_POWER;
	if (name == STR_OK) return KEY_OK;
	if (name == STR_BACK) return KEY_BACK;
	if (name == STR_CHAN_UP) return KEY_CHAN_UP;
	if (name == STR_CHAN_DWN) return KEY_CHAN_DWN;
	if (name == STR_HELP) return KEY_HELP;
	if (name == STR_MENU) return KEY_MENU;
	if (name == STR_GUIDE) return KEY_GUIDE;
	if (name == STR_INFO) return KEY_INFO;
	if (name == STR_TEXT) return KEY_TEXT;
	if (name == STR_MENU1) return KEY_MENU1;
	if (name == STR_MENU2) return KEY_MENU2;
	if (name == STR_DPAD_UP) return KEY_DPAD_UP;
	if (name == STR_DPAD_DOWN) return KEY_DPAD_DOWN;
	if (name == STR_DPAD_LEFT) return KEY_DPAD_LEFT;
	if (name == STR_DPAD_RIGHT) return KEY_DPAD_RIGHT;
	if (name == STR_NUM_0) return KEY_NUM_0;
	if (name == STR_NUM_1) return KEY_NUM_1;
	if (name == STR_NUM_2) return KEY_NUM_2;
	if (name == STR_NUM_3) return KEY_NUM_3;
	if (name == STR_NUM_4) return KEY_NUM_4;
	if (name == STR_NUM_5) return KEY_NUM_5;
	if (name == STR_NUM_6) return KEY_NUM_6;
	if (name == STR_NUM_7) return KEY_NUM_7;
	if (name == STR_NUM_8) return KEY_NUM_8;
	if (name == STR_NUM_9) return KEY_NUM_9;
	if (name == STR_PAUSE) return KEY_PAUSE;
	if (name == STR_STOP) return KEY_STOP;
	if (name == STR_RECORD) return KEY_RECORD;
	if (name == STR_FWD) return KEY_FWD;
	if (name == STR_RWD) return KEY_RWD;
	if (name == STR_MENU3) return KEY_MENU3;
	if (name == STR_UNKNOWN_0) return KEY_UNKNOWN_0;	// TIMESHIFT INFO
	if (name == STR_UNKNOWN_1) return KEY_UNKNOWN_1;  // POWER
	if (name == STR_UNKNOWN_2) return KEY_UNKNOWN_2;	// ID
	if (name == STR_UNKNOWN_3) return KEY_UNKNOWN_3;  // RC PAIRING
	if (name == STR_UNKNOWN_4) return KEY_UNKNOWN_4;  // TIMINGS
	if (name == STR_ONDEMAND) return KEY_ONDEMAND;
	if (name == STR_DVR) return KEY_DVR;
	if (name == STR_TV) return KEY_TV;
	return -1;
}


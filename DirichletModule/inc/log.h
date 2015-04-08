/**
 * @file log.h
 * @brief Very simple logging file
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2015 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Apr 8, 2015
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <fstream>

#define FATAL      0
#define ERROR      1
#define WARN       2
#define INFO       3
#define DEBUG      4
#define TRACE      5

/** 
 * To adjust overal verbosity, this is the only field you will have to adjust. If you have defined above logging
 * levels somewhere else, make sure the values are actually the same.
 */
#ifndef VERBOSITY
#define VERBOSITY INFO
#endif

/**
 * A very simple way to maintain the syntax of std::cout and still have the possibility to enable/disable logging
 * information.
 *
 * Usage: 
 *
 *   dobots::debug << "Hello world!" << std::endl;
 *   dobots::info << "What do you think? I'm Gaya?" << std::endl;
 *
 * Up to warning is sent over std::cout, above that is sent to std::cerr.
 */
namespace dobots {
	using namespace std;

	static ofstream devnull("/dev/null");

#define trace cout << "[" << __TIME__ << "] TRACE: " 
#define debug cout << "[" << __TIME__ << "] DEBUG: " 
#define info cout << "[" << __TIME__ << "] INFO: "
#define warn cout << "[" << __TIME__ << "] WARN: "
#define error cerr << "[" << __TIME__ << "] ERROR: "
#define fatal cerr << "[" << __TIME__ << "] FATAL: "

#if VERBOSITY < TRACE
#undef trace
#define trace devnull
#endif

#if VERBOSITY < DEBUG
#undef debug
#define debug devnull
#endif

#if VERBOSITY < INFO
#undef info
#define info devnull
#endif

#if VERBOSITY < WARN
#undef warn
#define warn devnull
#endif

#if VERBOSITY < ERROR
#undef error
#define error devnull
#endif

#if VERBOSITY < FATAL
#undef fatal
#define fatal devnull
#endif
}


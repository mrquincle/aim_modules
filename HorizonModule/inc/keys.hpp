/*
 * Copyright 2013 Rob Groenendijk
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HORIZON_HPP_
#define HORIZON_HPP_

#include <string>
#include <vector>

/* horizon specific keycodes found by manually scanning/testing the entire range */

#define DECLARE_KEY(name,value) \
		const int KEY_##name=value; \
		const std::string STR_##name=#name;

DECLARE_KEY(POWER            ,0xe000);
DECLARE_KEY(OK               ,0xe001);
DECLARE_KEY(BACK             ,0xe002);
DECLARE_KEY(CHAN_UP          ,0xe006);
DECLARE_KEY(CHAN_DWN	     ,0xe007);
DECLARE_KEY(HELP             ,0xe009);
DECLARE_KEY(MENU             ,0xe00a);
DECLARE_KEY(GUIDE            ,0xe00b);
DECLARE_KEY(INFO             ,0xe00e);
DECLARE_KEY(TEXT             ,0xe00f);
DECLARE_KEY(MENU1            ,0xe011);
DECLARE_KEY(MENU2            ,0xe015);
DECLARE_KEY(DPAD_UP          ,0xe100);
DECLARE_KEY(DPAD_DOWN	     ,0xe101);
DECLARE_KEY(DPAD_LEFT        ,0xe102);
DECLARE_KEY(DPAD_RIGHT       ,0xe103);
DECLARE_KEY(NUM_0            ,0xe300);
DECLARE_KEY(NUM_1            ,0xe301);
DECLARE_KEY(NUM_2            ,0xe302);
DECLARE_KEY(NUM_3            ,0xe303);
DECLARE_KEY(NUM_4            ,0xe304);
DECLARE_KEY(NUM_5            ,0xe305);
DECLARE_KEY(NUM_6            ,0xe306);
DECLARE_KEY(NUM_7            ,0xe307);
DECLARE_KEY(NUM_8            ,0xe308);
DECLARE_KEY(NUM_9            ,0xe309);
DECLARE_KEY(PAUSE            ,0xe400);
DECLARE_KEY(STOP             ,0xe402);
DECLARE_KEY(RECORD           ,0xe403);
DECLARE_KEY(FWD              ,0xe405);
DECLARE_KEY(RWD              ,0xe407);
DECLARE_KEY(MENU3            ,0xef00);
DECLARE_KEY(UNKNOWN_0        ,0xef06);	// TIMESHIFT INFO
DECLARE_KEY(UNKNOWN_1        ,0xef15);  // POWER
DECLARE_KEY(UNKNOWN_2        ,0xef16);	// ID
DECLARE_KEY(UNKNOWN_3        ,0xef17);  // RC PAIRING
DECLARE_KEY(UNKNOWN_4        ,0xef19);  // TIMINGS
DECLARE_KEY(ONDEMAND         ,0xef28);
DECLARE_KEY(DVR              ,0xef29);
DECLARE_KEY(TV               ,0xef2a);

const int getKey(const std::string name);

#endif /* HORIZON_HPP_ */

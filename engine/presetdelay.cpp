/*
	Korg Pandora PX5D Editor for Linux
	Copyright (C) 2011-2012, Frederic Jaume <dev@exp-media.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; If not, see <http://www.gnu.org/licenses/>.
*/

#include "presetdelay.h"

#include <stdio.h>
#include <sstream>

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] ))

const char* PresetDelay::commonValues[] = {
	"0.0", "0.3", "0.7",
	"1.0", "1.3", "1.7",
	"2.0", "2.3", "2.7",
	"3.0", "3.3", "3.7",
	"4.0", "4.3", "4.7",
	"5.0", "5.3", "5.7",
	"6.0", "6.3", "6.7",
	"7.0", "7.3", "7.7",
	"8.0", "8.3", "8.7",
	"9.0", "9.3", "9.7",
	"10.0"
};

const PresetDelay::DelayDef PresetDelay::Delays[PresetDelay::END] =  {
	{SLAP1,     "Slap1",    "Slap Delay 1"},
	{SLAP2,     "Slap2",    "Slap Delay 2" },
	{SLAP3,     "Slap3",    "Slap Delay 3" },
	{SLAP4,     "Slap4",    "Slap Delay 4" },
	{SLAP5,     "Slap5",    "Slap Delay 5" },
	{ECHO1,     "Echo1",    "Tape Echo 1" },
	{ECHO2,     "Echo2",    "Tape Echo 2" },
	{ECHO3,     "Echo3",    "Tape Echo 3" },
	{ECHO4,     "Echo4",    "Tape Echo 4" },
	{ECHO5,     "Echo5",    "Tape Echo 5" },
	{CLRDLY1,   "Clr Dly1", "Clear Delay 1" },
	{CLRDLY2,   "Clr Dly2", "Clear Delay 2" },
	{CLRDLY3,   "Clr Dly3", "Clear Delay 3" },
	{CLRDLY4,   "Clr Dly4", "Clear Delay 4" },
	{CLRDLY5,   "Clr Dly5", "Clear Delay 5" },
	{PPONGDLY1, "Ppdly1",   "Ping Pong Delay 1" },
	{PPONGDLY2, "Ppdly2",   "Ping Pong Delay 2" },
	{PPONGDLY3, "Ppdly3",   "Ping Pong Delay 3" },
	{PPONGDLY4, "Ppdly4",   "Ping Pong Delay 4" },
	{PPONGDLY5, "Ppdly5",   "Ping Pong Delay 5" },
	{MULTITAP1, "Multi1",   "Multi-Tap Delay 1" },
	{MULTITAP2, "Multi2",   "Multi-Tap Delay 2" },
	{MULTITAP3, "Multi3",   "Multi-Tap Delay 3" },
	{MULTITAP4, "Multi4",   "Multi-Tap Delay 4" },
	{MULTITAP5, "Multi5",   "Multi-Tap Delay 5" },
	{REVERSEDLY,  "Rvs Dly",  "Reverse Delay"  }

};

PresetDelay::PresetDelay()
{
	type = SLAP1;
	on = false;
	fxLevel = 10;
	timeMS = 0;
}

PresetDelay::DelayTypes PresetDelay::validateFx(DelayTypes m) {
	if ( m>=END ) {
		return REVERSEDLY;
	} else if ( m<SLAP1 ) {
		return SLAP1;
	} else {
		return m;
	}
}

unsigned int PresetDelay::validateParameter(unsigned int param, unsigned int value) {

	unsigned int pMin;
	unsigned int pMax;

	if ( param==2 ) {
		pMin = getMinParam2();
		pMax = getMaxParam2();
	} else {
		pMin = getMinParam1();
		pMax = getMaxParam1();
	}

	// Clamp parameter if necessary
	if ( value<pMin ) {
		return pMin;
	} else if ( value>pMax ) {
		return pMax;
	} else {
		return value;
	}
}

const char* PresetDelay::getParam1StringValue(unsigned int v) {
	return ( v < SIZEOF_ARRAY(commonValues) ) ? commonValues[v] : "?";
}

const char* PresetDelay::getParam2StringValue(unsigned int v) {

	// Delay values go from 0ms to 500ms on most effects
	// by steps of 5 ms
	// Some effects like Echo1 go frim 0 to 2 seconds by steps of 20 ms
	// Some effects like Clear Delay go frim 0 to 1 second by steps of 20 ms

	static char delayString[32];

	sprintf(delayString, "?");

	switch (type) {
	case ECHO1:
	case ECHO2:
	case ECHO3:
	case ECHO4:
	case ECHO5:
	case PPONGDLY1:
	case PPONGDLY2:
	case PPONGDLY3:
	case PPONGDLY4:
	case PPONGDLY5:
	case MULTITAP1:
	case MULTITAP2:
	case MULTITAP3:
	case MULTITAP4:
	case MULTITAP5:
		//TODO: when > 1000 display as seconds (fixed)
		sprintf(delayString, "%d ms", v * 20);
		break;

	case CLRDLY1:
	case CLRDLY2:
	case CLRDLY3:
	case CLRDLY4:
	case CLRDLY5:
	case REVERSEDLY:
		sprintf(delayString, "%d ms", v * 20);
		break;

	default:
		sprintf(delayString, "%d ms", v * 5);
	}

	return delayString;
}

const char* PresetDelay::getParam1Name() const {
	return "Effect Level";
}

const char* PresetDelay::getParam2Name() const {
	return "Time";
}

unsigned int PresetDelay::getMinParam1() {
	return 0;
}

unsigned int PresetDelay::getMaxParam1() {
	return 0x1e;
}

unsigned int PresetDelay::getMinParam2() {
	return 0;
}

unsigned int PresetDelay::getMaxParam2() {

	switch ( type ) {
	case CLRDLY1:
	case CLRDLY2:
	case CLRDLY3:
	case CLRDLY4:
	case CLRDLY5:
	case REVERSEDLY:
		return 0x32;
		break;

	default:
		return 0x64;
	}

}

bool PresetDelay::setDelay(DelayTypes dly) {
	type = validateFx(dly);
	return type==dly;
}

bool PresetDelay::setParameter1(unsigned int p) {
	fxLevel = validateParameter(1, p);
	return fxLevel==p;
}

bool PresetDelay::setParameter2(unsigned int p) {
	timeMS = validateParameter(2, p);
	return timeMS==p;
}

std::string PresetDelay::toString() const {
	std::stringstream ss;
	ss << (on?"ON":"OFF") << " FX=" << Delays[type].fullName << ", " ;
	ss << getParam1Name() << "= " << fxLevel << ", ";
	ss << getParam2Name() << "= " << timeMS << "";
	return ss.str();
}


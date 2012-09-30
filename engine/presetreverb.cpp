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
#include "presetreverb.h"

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] ))

const PresetReverb::ReverbDef PresetReverb::Reverbs[PresetReverb::END] =  {
	{DRYAIR,   "Dryair",   "Dry Air"},
	{WETAIR,   "Wetair",   "Wet Air"},
	{ROOM1,    "Room1",    "Room 1"},
	{ROOM2,    "Room2",    "Room 2"},
	{PLATE1,   "Plate1",   "Plate 1"},
	{PLATE2,   "Plate2",   "Plate 2"},
	{HALL1,    "Hall1",    "Hall 1"},
	{HALL2,    "Hall2",    "Hall 2"},
	{SPRING1,  "Spring1",  "Spring 1"},
	{SPRING2,  "Spring2",  "Spring 2"},
	{BRIGHT,   "Bright",   "Bright"}
};

const char* PresetReverb::commonValues[] = {
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

PresetReverb::PresetReverb() {
	type = DRYAIR;
	fxLevel = 0;
	on = false;
}


const char* PresetReverb::getParamStringValue(unsigned int v) {
	return ( v < SIZEOF_ARRAY(commonValues) ) ? commonValues[v] : "?";
}

bool PresetReverb::setReverb(ReverbTypes fx) {
	type = validateFx(fx);
	return type==fx;
}

bool PresetReverb::setParameter(unsigned int p) {
	fxLevel = validateParameter(p);
	return fxLevel==p;
}

const char* PresetReverb::getParamName() {
	return "Effect Level";
}

unsigned int PresetReverb::getMinParam() {
	return 0;
}

unsigned int PresetReverb::getMaxParam() {
	return 0x1e;
}

PresetReverb::ReverbTypes PresetReverb::validateFx(ReverbTypes m) {
	if ( m>=END ) {
		return BRIGHT;
	} else if ( m<DRYAIR ) {
		return DRYAIR;
	} else {
		return m;
	}
}

unsigned int PresetReverb::validateParameter(unsigned int value) {
	unsigned int pMin = getMinParam();
	unsigned int pMax = getMaxParam();

	// Clamp parameter if necessary
	if ( value<pMin ) {
		return pMin;
	} else if ( value>pMax ) {
		return pMax;
	} else {
		return value;
	}
}

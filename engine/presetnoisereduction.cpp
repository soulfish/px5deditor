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

#include "presetnoisereduction.h"
#include <sstream>

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] ))

const char* PresetNoiseReduction::commonValues[] = {
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

PresetNoiseReduction::PresetNoiseReduction() {
	noiseReductionLevel = 0;
}

const char* PresetNoiseReduction::getParamStringValue(unsigned int v) {
	return ( v < SIZEOF_ARRAY(commonValues) ) ? commonValues[v] : "?";
}

unsigned int PresetNoiseReduction::validateParameter(unsigned int value) {
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

bool PresetNoiseReduction::setParameter(unsigned int p) {
	noiseReductionLevel = validateParameter(p);
	return noiseReductionLevel==p;
}

const char* PresetNoiseReduction::getParamName() const {
	return "NR Level";
}

unsigned int PresetNoiseReduction::getMinParam() {
	return 0;
}

unsigned int PresetNoiseReduction::getMaxParam() {
	return 0x1e;
}

std::string PresetNoiseReduction::toString() const {
	std::stringstream ss;
	ss << ((noiseReductionLevel>0)?"ON":"OFF") << ", " ;
	ss << getParamName() << "= " << noiseReductionLevel;
	return ss.str();
}

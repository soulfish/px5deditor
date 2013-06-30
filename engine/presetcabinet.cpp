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

#include "presetcabinet.h"

#include <sstream>

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] ))

const char* PresetCabinet::commonValues[] = {
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

const PresetCabinet::CabDef PresetCabinet::Cabs[PresetCabinet::END] =  {
	{TWEED1x8, "1x8 Twd", "1x8\" Tweed" },
	{BLACK2x10, "2x10 Blk", "2x10\" Black" },
	{TWEED4x10, "4x10 Twd", "4x10\" Tweed" },
	{TWEED1x12, "1x12 Twd", "1x12\" Tweed" },
	{VOX1x12, "1x12 Vox", "1x12\" Vox" },
	{BLACK2x12, "2x12 Blk", "2x12\" Black" },
	{VOX2x12, "2x12 Vox", "2x12\" Vox"  },
	{VOX4x12, "4x12 Vox", "4x12\" Vox" },
	{CLS4x12, "4x12 Cls", "4x12\" Classic" },
	{MDN4x12, "4x12 Mdn", "4x12\" Modern" },
	{VIN4x12, "4x12 Vin", "4x12\" Vintage" },
	{LA4x10, "LA 4x10", "(bass) 4x10\" L.A." }, // bass
	{MDN4x10, "MDN 4x10", "(bass) 4x10\" Modern" },
	{MTL4x10, "MTL 4x10", "(bass) 4x10\" Metal" },
	{CLS8x10, "CLS 8x10", "(bass) 8x10\" Classic" },
	{UK4x12, "UK 4x12", "(bass) 4x12\" U.K." },
	{STU1x15, "STU 1x15", "(bass) 1x15\" Studio" },
	{JAZ1x15, "JAZ 1x15", "(bass) 1x15\" Jazz" },
	{AC2x15, "AC 2x15", "(bass) 2x15\" AC" },
	{US2x15, "US 2x15", "(bass) 2x15\" U.S." },
	{UK4x15, "UK 4x15", "(bass) 4x15\" U.K." },
	{LA1x18, "LA 1x18", "(bass) 1x18\" L.A."},
	{COMBI, "Combi", "Combi"}
};

PresetCabinet::PresetCabinet()
{
	model = TWEED1x8;
	presence = 0;
	on = false;
}

PresetCabinet::CabTypes PresetCabinet::validateModel(CabTypes m) {
	if ( m>=END ) {
		return COMBI;
	} else if ( m<TWEED1x8 ) {
		return TWEED1x8;
	} else {
		return m;
	}
}

unsigned int PresetCabinet::validateParameter(unsigned int value) {

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

const char* PresetCabinet::getParamStringValue(unsigned int v) {
	return ( v < SIZEOF_ARRAY(commonValues) ) ? commonValues[v] : "?";
}

bool PresetCabinet::setCabinet(CabTypes cab) {
	model = validateModel(cab);
	return model==cab;
}

bool PresetCabinet::setParameter(unsigned int p) {
	presence = validateParameter(p);
	return presence==p;
}

const char* PresetCabinet::getParamName() const {
	return "Presence";
}

unsigned int PresetCabinet::getMinParam() {
	return 0;
}

unsigned int PresetCabinet::getMaxParam() {
	return 0x1e;
}

std::string PresetCabinet::toString() const {

	std::stringstream ss;
	ss << (on?"ON":"OFF") << " FX=" << Cabs[model].fullName << ", " ;
	ss << getParamName() << "= " << presence;
	return ss.str();
}


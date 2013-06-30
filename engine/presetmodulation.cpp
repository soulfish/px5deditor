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

#include "presetmodulation.h"
#include <sstream>

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] ))

const PresetModulation::ModDef PresetModulation::Modulations[PresetModulation::END] =  {
	{ CHORUS1,    "Chorus1",    "Chorus 1 (L: fx / R: direct)",  "Speed"},
	{ CHORUS2,    "Chorus2",    "Chorus 2 (Vintage phase invert)",  "Speed" },
	{ FLANGE1,    "Flange1",    "Flanger 1 (Feedback 0%)", "Speed"},
	{ FLANGE2,    "Flange2",    "Flanger 2 (Feedback 22%)", "Speed"},
	{ FLANGE3,    "Flange3",    "Flanger 3 (Feedback 60%)", "Speed"},
	{ FLANGE4,    "Flange4",    "Flanger 4 (Phase invert feedback)", "Speed"},
	{ PHASER1,    "Phaser1",    "Phaser 1 (4-stage)",  "Speed"},
	{ PHASER2,    "Phaser2",    "Phaser 2 (thick mid-range)",  "Speed"},
	{ PHASER3,    "Phaser3",    "Phaser 3 (10-stage)",  "Speed"},
	{ TWELVESTR,  "12str",      "12-String Simulator", "Depth"},
	{ UVIBE1,     "U-Vibe1",    "Uni-Vibe 1 (chorus mode)",  "Speed"},
	{ UVIBE2,     "U-Vibe2",    "Uni-Vibe 2 (vibrato mode)",  "Speed"},
	{ TREM1,      "Trem1",		"Tremolo 1 (smooth)",   "Speed"},
	{ TREM2,      "Trem2",      "Tremolo 2 (guitar amp)",   "Speed"},
	{ TREM3,      "Trem3",      "Tremolo 3 (strong)",   "Speed"},
	{ AUTOPAN1,   "At Pan1",    "Auto Pan 1 (LFO: square)",  "Speed"},
	{ AUTOPAN2,   "At Pan2",    "Auto Pan 2 (LFO: sine)",  "Speed"},
	{ ROTARY1,    "Rotary1",    "Rotary Speaker 1",  "Speed"},
	{ ROTARY2,    "Rotary2",    "Rotary Speaker 2 (dynamic)",  "Threshold"},
	{ ROTARY3,    "Rotary3",    "Rotary Speaker 3 (dynamic)",  "Threshold"},
	{ PITCH1,     "Pitch1",     "Pitch Shifter 1 (Level 1)",  "Pitch Shift"},
	{ PITCH2,     "Pitch2",     "Pitch Shifter 2 (Level 2)",  "Pitch Shift"},
	{ PITCH3,     "Pitch3",     "Pitch Shifter 3 (Level 3)",  "Pitch Shift"},
	{ PITCH4,     "Pitch4",     "Pitch Shifter 4 (FX Only)",  "Pitch Shift"},
	{ MAJ3UP,     "Maj3up",     "Harmonizer: Major 3rd +", "Key" },
	{ MAJ3DOWN,   "Maj3dwn",    "Harmonizer: Major 3rd -", "Key"},
	{ MIN3UP,     "Min3up",     "Harmonizer: minor 3rd +", "Key"},
	{ MIN3DOWN,   "Min3dwn",    "Harmonizer: minor 3rd -", "Key"},
	{ PERF4,      "Per4th",     "Harmonizer: Perfect 4th +", "Key"},
	{ PERF5,      "Per5th",     "Harmonizer: Perfect 5th +", "Key"},
	{ MAJ6,       "Maj6th",     "Harmonizer: Major 6th +", "Key"},
	{ FEEDBACK1,  "Feedbk1",    "Feedback 1",   "Depth"},
	{ FEEDBACK2,  "Feedbk2",    "Feedback 2 (octave up)",   "Depth"},
	{ FEEDBACK3,  "Feedbk3",    "Feedback 3",   "Depth"},
	{ FEEDBACK4,  "Feedbk4",    "Feedback 4 (octave up)",   "Depth"},
	{ FILTER,     "Filter",     "Filter (fixed)",       "Frequency"},
	{ FILTERUP1,  "Filtup1",    "Auto-Filter Up 1",   "Sensibilty"},
	{ FILTERUP2,  "Filtup2",    "Auto-Filter Up 2",   "Sensibilty"},
	{ FILTERDWN1, "Filtdn1",    "Auto-Filter Down 1", "Sensibilty"},
	{ FILTERDWN2, "Filtdn2",    "Auto-Filter Down 2", "Sensibilty"},
	{ TALK1,      "Talk1",      "Talkbox 1 (YAH-YAH)",     "Sensibilty"},
	{ TALK2,      "Talk2",      "Talkbox 2 (U-WAH)",     "Sensibilty"},
	{ TALK3,      "Talk3",      "Talkbox 3 (YOI-YOI)",     "Sensibilty"},
	{ TALK4,      "Talk4",      "Talkbox 4 (YEAH)",     "Sensibilty"},
	{ RANDOM1,    "Random1",    "Random 1",     "Speed"},
	{ RANDOM2,    "Random2",    "Random 2",     "Speed"},
	{ RANDOM3,    "Random3",    "Random 3",     "Speed"},
	{ RANDOM4,    "Random4",    "Random 4",     "Speed"},
	{ DRONE1,     "Drone1",     "Drone 1 (soft)",      "Key"},
	{ DRONE2,     "Drone2",     "Drone 2 (strong)",      "Key"},
	{ ENVPITCH,   "Env Pit",    "Envelope Pitch (dynamic)",    "Depth"},
	{ ENVCHORUS,  "Env Cho",    "Envelope Chorus (dynamic)",   "Depth"},
	{ INFFLANGE1, "Inffln1",    "Infinite Flanger 1", "Depth"},
	{ INFFLANGE2, "Inffln2",    "Infinite Flanger 2", "Depth"},
	{ INFPITCH1,  "Infpit1",    "Infinite Pitch 1",   "Depth"},
	{ INFPITCH2,  "Infpit2",    "Infinite Pitch 2",   "Depth"}
};

const char* PresetModulation::chorusValues[] = {
	"0.10 Hz", "0.12 Hz", "0.14 Hz", "0.16 Hz", "0.18 Hz",
	"0.20 Hz", "0.26 Hz", "0.32 Hz", "0.38 Hz", "0.4 Hz",
	"0.5 Hz", "0.6 Hz",   "0.7 Hz", "0.8 Hz", "0.9 Hz",
	"1.0 Hz", "1.2 Hz", "1.4 Hz", "1.6 Hz", "1.8 Hz",
	"2.0 Hz", "2.6 Hz", "3.2 Hz", "3.8 Hz", "4.4 Hz",
	"5.0 Hz", "6.0 Hz", "7.0 Hz", "8.0 Hz", "9.0 Hz",
	"10.0 Hz"
};

const char* PresetModulation::commonValues[] = {
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

//TODO: Check why -17 and +19
//TODO: add name of intervals
const char* PresetModulation::pitchValues[] = {
	"-24", "-17", "-12", "-11", "-10",
	"-9", "-8", "-7", "-6", "-5",
	"-4", "-3", "-2", "-1", "-d",
	"d", "1", "2", "3", "4",
	"5", "6", "7", "8", "9",
	"10", "11", "12", "19", "24"
};

//TODO: add alternative names of scales
const char* PresetModulation::scaleValues[] = {
	"C", "C#", "D", "D#", "E",
	"F", "F#", "G", "G#", "A",
	"A#", "B"
};

const char* PresetModulation::envelopeValues[] = {
	"-15", "-14", "-13", "-12", "-11",
	"-10", "-9", "-8", "-7", "-6",
	"-5", "-4", "-3", "-2", "-1",
	"0", "1", "2", "3", "4",
	"5", "6", "7", "8", "9",
	"10", "11", "12", "13", "14",
	"15"
};


PresetModulation::PresetModulation() {
	effect = CHORUS1;
	on = false;
	param = 0;
}

const char* PresetModulation::getParamStringValue(unsigned int v) {

	switch ( effect ) {

	case CHORUS1:
	case CHORUS2:
	case FLANGE1:
	case FLANGE2:
	case FLANGE3:
	case FLANGE4:
	case PHASER1:
	case PHASER2:
	case PHASER3:

	case UVIBE1:
	case UVIBE2:
	case TREM1:
	case TREM2:
	case TREM3:
	case AUTOPAN1:
	case AUTOPAN2:
	case ROTARY1:

	case FEEDBACK1:
	case FEEDBACK2:
	case FEEDBACK3:
	case FEEDBACK4:

	case FILTERUP1:
	case FILTERUP2:
	case FILTERDWN1:
	case FILTERDWN2:
	case TALK1:
	case TALK2:
	case TALK3:
	case TALK4:
	case INFFLANGE1:
	case INFFLANGE2:
		return ( v < SIZEOF_ARRAY(chorusValues) ) ? chorusValues[v] : "?";

	case ROTARY2:
	case ROTARY3:
	case FILTER:
	case RANDOM1:
	case RANDOM2:
	case RANDOM3:
	case RANDOM4:
		return ( (v+3) < SIZEOF_ARRAY(chorusValues) ) ? chorusValues[v+3] : "?";

	case PITCH1:
	case PITCH2:
	case PITCH3:
	case PITCH4:
	case INFPITCH1:
	case INFPITCH2:
		return ( v < SIZEOF_ARRAY(pitchValues) ) ? pitchValues[v] : "?";

	case MAJ3UP:
	case MAJ3DOWN:
	case MIN3UP:
	case MIN3DOWN:
	case PERF4:
	case PERF5:
	case MAJ6:
	case DRONE1:
	case DRONE2:
		return ( v < SIZEOF_ARRAY(scaleValues) ) ? scaleValues[v] : "?";

	case ENVPITCH:
	case ENVCHORUS:
		return ( v < SIZEOF_ARRAY(envelopeValues) ) ? envelopeValues[v] : "?";

	case TWELVESTR:
		return ( v < SIZEOF_ARRAY(commonValues) ) ? commonValues[v] : "?";

	default:
		return "?";
	}
}

bool PresetModulation::setModulation(ModulationTypes mod) {
	effect = validateFx(mod);
	return effect==mod;
}

const char* PresetModulation::getParamName() const {
	return Modulations[effect].paramName.c_str();
}

unsigned int PresetModulation::getMinParam() {
	return 0;
}

unsigned int PresetModulation::getMaxParam() {

	switch ( effect ) {
	case ROTARY2:
	case ROTARY3:
	case PITCH1: // TODO: to check!
	case PITCH2:
	case PITCH3:
	case PITCH4:
	case RANDOM1:
	case RANDOM2:
	case RANDOM3:
	case RANDOM4:
	case INFPITCH1:
	case INFPITCH2:
		return 0x1d;

	case MAJ3UP:
	case MAJ3DOWN:
	case MIN3UP:
	case MIN3DOWN:
	case PERF4:
	case PERF5:
	case MAJ6:
	case DRONE1:
	case DRONE2:
		return 0xb; // TODO: check!!

	default:
		return 0x1e;

	}
}

bool PresetModulation::setParameter(unsigned int p) {
	param = validateParameter(p);
	return param==p;
}

PresetModulation::ModulationTypes PresetModulation::validateFx(PresetModulation::ModulationTypes m) {
	if ( m>=END ) {
		return INFPITCH2;
	} else if ( m<CHORUS1 ) {
		return CHORUS1;
	} else {
		return m;
	}
}

unsigned int PresetModulation::validateParameter(unsigned int value) {

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

std::string PresetModulation::toString() const {
	std::stringstream ss;
	ss << (on?"ON":"OFF") << " FX=" << Modulations[effect].fullName << ", " ;
	ss << getParamName() << "= " << param;
	return ss.str();
}




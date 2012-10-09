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

#include "presetdynamics.h"

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] ))

const char* PresetDynamics::commonValues[] = {
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

const PresetDynamics::EffectDef PresetDynamics::Effects[PresetDynamics::END] =  {
	{COMPRESSOR, "Comp", "Compressor", "Sensibility"},
	{DYNAMICEXCITER,"DynaExc", "Dynamic Exciter", "Sensibility"},
	{VOXWAH,"VowWah", "Vox Wah (Vox V847)", "Frequency"},
	{WAHUP1,"WahUp1", "Auto-Wah Up 1", "Sensibility",},
	{WAHUP2,"WahUp2", "Auto-Wah Up 2", "Sensibility"},
	{WAHDOWN1,"WahDown1", "Auto-Wah Down 1", "Sensibility"},
	{WAHDOWN2,"WahDown2", "Auto-Wah Down 2", "Sensibility"},
	{SLOWATTACK,"SlowAtk", "Slow Attack", "Attack"},
	{ACOUSTICSIM,"AcSim", "Acoustic Simulator", "Tone"},
	{RESONATOR,"Reso", "Resonator", "Body"},
	{OCTAVE,"Octave", "Octaver", "FX Level"},
	{RING,"Ring", "Ring Modulator", "Freq"},
	{RINGUP,"RingUp", "Ring Up", "Sensibility"},
	{RINGDOWN,"RingDwn", "Ring Down", "Sensibility"},
	{CRUSHER,"Crusher", "Crusher", "Gain"},
	{HUM2SINGLE,"Hum>Sgl", "Humbucker to Single Coil", "Tone"},
	{SINGLE2HUM,"Sgl>Hum", "Single Coil to Humbucker", "Tone"},
	{HALFTONE,"Hf Tone", "Half Tone", "Tone"},
	{PHASEOUT,"Paseout", "Phase Out", "Tone"},
	{BASSCOMP,"B Comp", "Bass Compressor", "Sensibility"},
	{LIMITER,"Limiter", "Limiter", "Sensibility"},
	{BASSOVERDRIVE,"B Od", "Bass Overdrive", "Gain"},
	{BASSDIST,"B Dist", "Bass Distortion", "Gain"},
	{BASSFUZZ,"B Fuzz", "Bass Fuzz", "Gain"},
	{PREAMP1,"Preamp1", "Preamp 1", "Tone"},
	{PREAMP2,"Preamp2", "Preamp 2", "Tone"},
	{BALANCE,"Balance", "Balance", "Tone"},
	{LOWCUT,"Low Cut", "Low Cut Filter", "Tone"},
	{FRETLESS,"Fretless", "Fretless", "Depth"}
};

PresetDynamics::PresetDynamics() {
	effect = COMPRESSOR;
	parameter = 0;
}

const char* PresetDynamics::getParamName() {
	return Effects[effect].parameterName.c_str();
}

unsigned int PresetDynamics::getMinParam() {
	return 0;
}

unsigned int PresetDynamics::getMaxParam() {

	switch (effect) {
	case COMPRESSOR:
	case VOXWAH:
	case SLOWATTACK:
	case RING:
	case BASSCOMP:
	case BASSOVERDRIVE:
	case BASSDIST:
	case BASSFUZZ:
		return 0x1b;
	default:
		return 0x1e;
	}

}

/*
 * Convert from internal parameter value to GUI float value
 * Usually between 0 and 10
 */
const char* PresetDynamics::getParamStringValue(unsigned int value) {

	int index = value;

	// The following parameters range from 1 to 10 instead of 0 to 10
	switch ( effect ) {
	case COMPRESSOR:
	case VOXWAH:
	case SLOWATTACK:
	case RING:
	case BASSCOMP:
	case BASSOVERDRIVE:
	case BASSDIST:
	case BASSFUZZ:
		index += 3;
		break;
	}
	return ( index < SIZEOF_ARRAY(commonValues) ) ? commonValues[index] : "?";

}

bool PresetDynamics::setEffect(DynamicsTypes fx) {
	effect = validateFx(fx);
	return effect==fx;
}

bool PresetDynamics::setParameter(unsigned int p) {
	parameter = validateParameter(p);
	return parameter==p;
}

PresetDynamics::DynamicsTypes PresetDynamics::validateFx(DynamicsTypes m) {
	if ( m>=END ) {
		return FRETLESS;
	} else if ( m<COMPRESSOR ) {
		return COMPRESSOR;
	} else {
		return m;
	}
}

unsigned int PresetDynamics::validateParameter(unsigned int value) {

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

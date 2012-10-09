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

#include "presetamp.h"

// NOTE: on day we could transform this and other tables to an XML file
#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] ))

const PresetAmp::AmpDef PresetAmp::Amps[PresetAmp::END] =  {

	{BTQCLN, "Btq Cln", "Boutique Clean", "Custom 100-Watt amp, Clean Channel"  },
	{BTQOD, "Btq Od", "Boutique Overdrive" , "Custom 100-Watt amp, Overdrive Channel"},
	{TWD1X12,"Twd1x12", "Tweed 1x12", "Fender Combo" },
	{TWD4X10, "Twd4x10", "Tweed 4x10", "Fender Combo, design for bass" },
	{BLK2X12, "Blk2x12", "Black 2x12 Combo","Good for Jazz/Blues" },
	{AC15, "Ac15", "AC15", "Vox AC15 from 1962" },
	{AC15TB, "Ac15tb", "AC15TB", "Vox AC15TB, Brilliant Channel" },
	{AC30, "Ac30", "AC30", "Vox AC30" },
	{AC30TB, "Ac30tb", "AC30TB", "Vox AC30TB, Brilliant Channel"},
	{UKBLUES, "Ukblues", "UK Blues", "Marshall Vintage Head"},
	{UK70S,"Uk‚70s", "UK '70s", "Marshall High Treble Channel, 1969" },
	{UK80S,"Uk‚80s", "UK '80s", "Marshall with Master Volume, 1983" },
	{UK90S,"Uk‚90s", "UK '90s", "Marshall Lead Channel, 90's amp'" },
	{UKMDN,"Uk Mdn", "UK Modern", "Marshall Modern" },
	{USHIG,"Us Hi-G", "US High Gain", "Soldano Snakeskin, 1991" },
	{USMDN,"Us Mdn", "US Modern", "Mesa Boogie" },
	{FUZZ,"Fuzz", "Fuzz", "Vintage 60's Fuzz" },
	{OCTFUZZ,"Octfuzz", "Octa Fuzz", "Emphasizes high notes" },

	{VALVE1,"Valve1", "Valve 1", "Tube bass amp with Ultra-Lo on" },
	{VALVE2,"Valve2", "Valve 2", "Tube bass amp for rock" },
	{CLASSIC,"Classic", "Classic", "Versatile tube bass amp" },
	{SCOOPED,"Scooped", "Scooped", "Bass amp from the 80's" },
	{LASTUD,"La Stud", "L.A. Studio", "Bass amp, L.A. Sound" },
	{GOLDPNL,"Goldpnl", "Gold Panel", "Bass amp, Eden Traveller" },
	{JAZZ,"Jazz", "Jazz", "Combo bass amp" },

	{STUDIO,"Studio", "Studio", "Tube combo bass amp" },
	{AC100,"Ac100", "AC100", "Vox AC100 Tube amp" },
	{UKMAJOR,"Ukmajor", "UK Major", "Marshall 200-Watt Tube bass amp" },

	{GSYNTH1,"Gsynth1", "Guitar Synth Variation 1", "(better with Cabinet off)" },
	{GSYNTH2,"Gsynth2", "Guitar Synth Variation 2", "(better with Cabinet off)" },
	{GSYNTH3,"Gsynth3", "Guitar Synth Variation 3", "(better with Cabinet off)" },
	{BSYNTH1,"Bsynth1", "Bass Synth Variation 1", "(better with Cabinet off)" },
	{BSYNTH2,"Bsynth2", "Bass Synth Variation 2", "(better with Cabinet off)" },
	{BSYNTH3,"Bsynth3", "Bass Synth Variation 3", "(better with Cabinet off)" },

};


// Each parameter value has a specific gui-oriented value
// like "0", "0.7", "+1 SIN" ...

// Common value for most settings 0->30
const char* PresetAmp::commonValues[] = {
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

const char* PresetAmp::midFreqValues[] = {
	"1", "2", "3", "4", "5"
};

const char* PresetAmp::synthDepthValues[] = {
	"-15", "-14", "-13", "-12", "-11",
	"-10", "-9", "-8", "-7", "-6",
	"-5", "-4", "-3", "-2", "-1",
	"0", "1", "2", "3", "4",
	"5", "6", "7", "8", "9",
	"10", "11", "12", "13", "14",
	"15"
};

/* TODO: actual Gsynth1,2,3 Wave values are:
 "-2 SAW", "-1 SAW", "-1 SIN", "SAW", "SIN"
 "+1 SAW", "+1 SIN", "+2 SIN", "-2 SAW2","-1 SAW2"
 "-1 SIN2", "SAW2", "SIN2", "+1 SAW2", "+1 SIN2", "+2 SIN2"
 */
const char* PresetAmp::synthWaveValues[] = {
	"Sawtooth -2oct", "Sawtooth -1oct", "Sine -1oct", "Sawtooth", "Sine",
	"Sawtooth +1oct", "Sine +1oct", "Sine +2oct",
	"Porta Sawtooth -2oct", "Porta Sawtooth -1oct", "Porta Sine -1oct", "Porta Sawtooth", "Porta Sine",
	"Porta Sawtooth +1oct", "Porta Sine +1oct",	"Porta Sine +2oct"
};


PresetAmp::PresetAmp()
{
	model = BTQCLN;
	gain = 0;
	bass = 0;
	treble = 0;
	volume = 0;
	middle = 0;
	on = true;

}
/*
// TODO: check if these are really usefull in the API
bool PresetAmp::isValidAmpModel(AmpTypes m) {
	return  m>=BTQCLN && m<END;
}

bool PresetAmp::isValidParamType(ParamTypes p) {
	return p>=AMP_GAIN && p<=AMP_MIDDLE;
}

bool PresetAmp::isValidParamValue(AmpTypes m, ParamTypes p, int v) {

	return ( v>=getParamMin(m, p) && v<=getParamMax(m, p) );
}
*/

/*
 * Ensure that a model is valid
 */
PresetAmp::AmpTypes PresetAmp::validateModel(AmpTypes m) {
	if ( m>=END ) {
		return BSYNTH3;
	} else if ( m<BTQCLN ) {
		return BTQCLN;
	} else {
		return m;
	}
}

/*
 * Ensure that a parameter for current model is valie
 */
unsigned int PresetAmp::validateParameter(ParamTypes param, unsigned int value) {

	unsigned int pMin = getMinParam(model, param);
	unsigned int pMax = getMaxParam(model, param);

	// Clamp parameter if necessary
	if ( value<pMin ) {
		return pMin;
	} else if ( value>pMax ) {
		return pMax;
	} else {
		return value;
	}
}

/*
 * Changes the current amp model.
 * If model was not valid, return false
 * and set a valid model.
 * TODO: decide if we should keep the previous model instead ?
 */
bool PresetAmp::setModel(AmpTypes m) {
	model = validateModel(m);


	// In an offline model, it should be important to ensure that the parameters remain valid
	// when we change the amp model!
	// When connected to the Pandora, the unit send corrected values itself...

	// gain = setGain(gain);
	// bass = setBass(bass);
	// treble = setTreble(treble);
	// 	middle = setMiddle(middle);
	// volume = setVolume(volume);

	return m==model;
}

bool PresetAmp::setGain(unsigned int g) {
	gain = validateParameter(AMP_GAIN, g);
	return g==gain;
}

bool PresetAmp::setBass(unsigned int b) {
	bass = validateParameter(AMP_BASS, b);
	return b==bass;
}

bool PresetAmp::setTreble(unsigned int t) {
	treble = validateParameter(AMP_TREBLE, t);
	return t==treble;
}

bool PresetAmp::setVolume(unsigned int v) {
	volume = validateParameter(AMP_VOLUME, v);
	return v==volume;
}

bool PresetAmp::setMiddle(unsigned int m) {
	middle = validateParameter(AMP_MIDDLE, m);
	return m==middle;
}

void PresetAmp::enable(bool flag) {
	on = flag;
}

unsigned int PresetAmp::getMinParam(AmpTypes /* m */, ParamTypes /*p */) {
	// All parameters have a 0 min value
	return 0;
}

unsigned int PresetAmp::getMaxParam(AmpTypes m, ParamTypes p) {
	switch (m) {
	case BTQCLN:
	case BTQOD:
	case TWD1X12:
	case TWD4X10:
	case BLK2X12:
	case AC15:
	case AC15TB:
	case AC30:
	case AC30TB:
	case UKBLUES:
	case UK70S:
	case UK80S:
	case UK90S:
	case UKMDN:
	case USHIG:
	case USMDN:
	case FUZZ:
	case OCTFUZZ:
	case STUDIO:
	case AC100:
	case UKMAJOR:
		return 0x1e;
		break;

	case VALVE1:
	case VALVE2:
	case CLASSIC:
	case SCOOPED:
	case LASTUD:
	case GOLDPNL:
	case JAZZ:
		if ( p==AMP_MIDDLE ) {
			return 4;
		} else {
			return 0x1e;
		}
		return 0;
		break;

	case GSYNTH1:
	case GSYNTH2:
	case GSYNTH3:
		if ( p==AMP_TREBLE ) {
			return 0x0f;
		} else {
			return 0x1e;
		}

	case BSYNTH1:
	case BSYNTH2:
	case BSYNTH3:
	default:
		return 0x1e;
		break;
	}

}

const char* PresetAmp::getParamName(ParamTypes p) {
	return getParamName(model, p);
}

const char* PresetAmp::getParamName(AmpTypes m, ParamTypes p) {
	// Amp has 5 parameters
	switch (m) {
	case BTQCLN:
	case BTQOD:
	case TWD1X12:
	case TWD4X10:
	case BLK2X12:
	case AC15:
	case AC15TB:
	case AC30:
	case AC30TB:
	case UKBLUES:
	case UK70S:
	case UK80S:
	case UK90S:
	case UKMDN:
	case USHIG:
	case USMDN:
	case FUZZ:
	case OCTFUZZ:
	case STUDIO:
	case AC100:
	case UKMAJOR:
		switch (p) {
		case AMP_GAIN:
			return "Gain";
			break;
		case AMP_BASS:
			return "Bass";
			break;
		case AMP_TREBLE:
			return "Treble";
			break;
		case AMP_VOLUME:
			return "Volume";
			break;
		case AMP_MIDDLE:
			return "Middle";
			break;
		}
		break;

	case VALVE1:
	case VALVE2:
	case CLASSIC:
	case SCOOPED:
	case LASTUD:
	case GOLDPNL:
	case JAZZ:
		switch (p) {
		case AMP_GAIN:
			return "Middle";
			break;
		case AMP_BASS:
			return "Bass";
			break;
		case AMP_TREBLE:
			return "Treble";
			break;
		case AMP_VOLUME:
			return "Volume";
			break;
		case AMP_MIDDLE:
			return "Mid Frequency";
			break;
		}
		break;

	case GSYNTH1:
	case GSYNTH2:
	case GSYNTH3:
	case BSYNTH1:
	case BSYNTH2:
	case BSYNTH3:
		switch (p) {
		case AMP_GAIN:
			return "Depth";
			break;
		case AMP_BASS:
			return "Decay";
			break;
		case AMP_TREBLE:
			return "Wave";
			break;
		case AMP_VOLUME:
			return "Synth Level";
			break;
		case AMP_MIDDLE:
			return "Direct Level";
			break;
		}
		break;

	default:
		return "Unknown parameter!!";
		break;
	}

}

const char* PresetAmp::getParamStringValue(ParamTypes p, unsigned int v) {
	return getParamStringValue(model, p, v);
}

const char* PresetAmp::getParamStringValue(AmpTypes m, ParamTypes p, unsigned int v) {

	switch (m) {
	case BTQCLN:
	case BTQOD:
	case TWD1X12:
	case TWD4X10:
	case BLK2X12:
	case AC15:
	case AC15TB:
	case AC30:
	case AC30TB:
	case UKBLUES:
	case UK70S:
	case UK80S:
	case UK90S:
	case UKMDN:
	case USHIG:
	case USMDN:
	case FUZZ:
	case OCTFUZZ:
	case STUDIO:
	case AC100:
	case UKMAJOR:
		// Fixme: instead of testing this, refactor to only getParamStringValue of
		// current parameter, which value is checked.
		return ( v < SIZEOF_ARRAY(commonValues) ) ? commonValues[v] : "?";

	case VALVE1:
	case VALVE2:
	case CLASSIC:
	case SCOOPED:
	case LASTUD:
	case GOLDPNL:
	case JAZZ:
		if ( p==AMP_MIDDLE ) {
			return ( v < SIZEOF_ARRAY(midFreqValues) ) ? midFreqValues[v] : "?";
		} else {
			return ( v < SIZEOF_ARRAY(commonValues) ) ? commonValues[v]	: "?";
		}
		break;

	case GSYNTH1:
	case GSYNTH2:
	case GSYNTH3:
		// gain = depth -15 / +15
		// bass = decay 0.0 / 10.0
		// treble = wave 0 / 15
		// volume = synth level 0.0 / 10.0
		// middle = direct level 0.0 / 10.0

		if ( p==AMP_GAIN ) {
			return ( v < SIZEOF_ARRAY(synthDepthValues) )  ? synthDepthValues[v] : "?";
		} else if ( p==AMP_TREBLE) {
			return ( v < SIZEOF_ARRAY(synthWaveValues) ) ? synthWaveValues[v] : "?";
		} else {
			//AMP_BASS, AMP_VOLUME, AMP_MIDDLE
			return ( v < SIZEOF_ARRAY(commonValues) ) ? commonValues[v] : "?";
		}

	case BSYNTH1:
	case BSYNTH2:
	case BSYNTH3:
		// gain = depth -15 / +15
		// bass = decay 0.0 / 10.0
		// treble = wave 0.0 / 10.0
		// volume = synth level 0.0 / 10.0
		// middle = direct level 0.0 / 10.0

		//  "Decay", 0, 0x1e, "Wave", 0, 15, "Synth Level", 0, 0x1e, "Direct Level", 0, 0x1e},
		if ( p==AMP_GAIN ) {
			return ( v < SIZEOF_ARRAY(synthDepthValues) )  ? synthDepthValues[v] : "?";
		} else {
			//AMP_BASS, AMP_VOLUME, AMP_MIDDLE
			return ( v < SIZEOF_ARRAY(commonValues) ) ? commonValues[v] : "?";
		}
		break;

	}




}



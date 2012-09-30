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
#ifndef PRESETAMP_H
#define PRESETAMP_H

#include <string>
#include <vector>

using namespace std;

class PresetAmp
{
public:

	enum AmpTypes {
		BTQCLN,
		BTQOD,
		TWD1X12,
		TWD4X10,
		BLK2X12,
		AC15,
		AC15TB,
		AC30,
		AC30TB,
		UKBLUES,
		UK70S,
		UK80S,
		UK90S,
		UKMDN,
		USHIG,
		USMDN,
		FUZZ,
		OCTFUZZ,
		VALVE1,
		VALVE2,
		CLASSIC,
		SCOOPED,
		LASTUD,
		GOLDPNL,
		JAZZ,
		STUDIO,
		AC100,
		UKMAJOR,
		GSYNTH1,
		GSYNTH2,
		GSYNTH3,
		BSYNTH1,
		BSYNTH2,
		BSYNTH3,
		END
	};

	enum ParamTypes {
		AMP_GAIN,
		AMP_BASS,
		AMP_TREBLE,
		AMP_VOLUME,
		AMP_MIDDLE
	};

	PresetAmp();

	AmpTypes getModel() { return model; }
	unsigned int getGain() { return gain; }
	unsigned int getBass() { return bass; }
	unsigned int getTreble() { return treble; }
	unsigned int getVolume() { return volume; }
	unsigned int getMiddle() { return middle; }
	bool getEnabled() { return on; }

	bool setModel(AmpTypes m);
	bool setGain(unsigned int g);
	bool setBass(unsigned int b);
	bool setTreble(unsigned int t);
	bool setVolume(unsigned int v);
	bool setMiddle(unsigned int m);
	void enable(bool flag);

	unsigned int getMinParam(AmpTypes m, ParamTypes p);
	unsigned int getMaxParam(AmpTypes m, ParamTypes p);
	const char* getParamName(AmpTypes m, ParamTypes p);

	// FIXME: strip AmpTypes from all these functions, use model!
	const char* getParamName(ParamTypes p);

	const char* getParamStringValue(ParamTypes p, unsigned int v);
	const char* getParamStringValue(AmpTypes m, ParamTypes p, unsigned int v);

	struct AmpDef {
		AmpTypes index;
		string name;
		string fullName;
	};
	static const AmpDef Amps[END];

private:

	AmpTypes model;
	unsigned int gain;
	unsigned int bass;
	unsigned int treble;
	unsigned int volume;
	unsigned int middle;

	bool on;

	AmpTypes validateModel(AmpTypes m);
	unsigned int validateParameter(ParamTypes param, unsigned int value);

	/*bool isValidAmpModel(AmpTypes m);
	bool isValidParamType(ParamTypes p);
	bool isValidParamValue(AmpTypes m, ParamTypes p, int v);
	*/

	static const char* commonValues[];
	static const char* midFreqValues[];
	static const char* synthDepthValues[];
	static const char* synthWaveValues[];


};

#endif // PRESETAMP_H

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
#ifndef PANDORAPRESET_H
#define PANDORAPRESET_H

#include <string>

#include "presetamp.h"
#include "presetcabinet.h"
#include "presetdelay.h"
#include "presetdynamics.h"
#include "presetmodulation.h"
#include "presetnoisereduction.h"
#include "presetreverb.h"

using namespace std;

class PandoraPreset
{
public:
	PandoraPreset();
	PandoraPreset(const PandoraPreset& orig);
	virtual ~PandoraPreset();

	static char translatePandoraCharacter(const char c);

	PresetDynamics dynamics;
	PresetAmp amp;
	PresetCabinet cabinet;
	PresetDelay delay;
	PresetModulation modulation;
	PresetNoiseReduction noiseReduction;
	PresetReverb reverb;

	enum ProgramBank {
		PROGRAM_FACTORY,
		PROGRAM_USER
	};

	string name;
	int number;
	ProgramBank slot;

private:
};

#endif // PANDORAPRESET_H

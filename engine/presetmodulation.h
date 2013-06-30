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
#ifndef PRESETMODULATION_H
#define PRESETMODULATION_H

#include <string>
using namespace std;

class PresetModulation
{
public:
	PresetModulation();

	enum ModulationTypes {
		CHORUS1,
		CHORUS2,
		FLANGE1,
		FLANGE2,
		FLANGE3,
		FLANGE4,
		PHASER1,
		PHASER2,
		PHASER3,
		TWELVESTR,
		UVIBE1,
		UVIBE2,
		TREM1,
		TREM2,
		TREM3,
		AUTOPAN1,
		AUTOPAN2,
		ROTARY1,
		ROTARY2,
		ROTARY3,
		PITCH1,
		PITCH2,
		PITCH3,
		PITCH4,
		MAJ3UP,
		MAJ3DOWN,
		MIN3UP,
		MIN3DOWN,
		PERF4,
		PERF5,
		MAJ6,
		FEEDBACK1,
		FEEDBACK2,
		FEEDBACK3,
		FEEDBACK4,
		FILTER,
		FILTERUP1,
		FILTERUP2,
		FILTERDWN1,
		FILTERDWN2,
		TALK1,
		TALK2,
		TALK3,
		TALK4,
		RANDOM1,
		RANDOM2,
		RANDOM3,
		RANDOM4,
		DRONE1,
		DRONE2,
		ENVPITCH,
		ENVCHORUS,
		INFFLANGE1,
		INFFLANGE2,
		INFPITCH1,
		INFPITCH2,
		END
	};

	struct ModDef {
		ModulationTypes index;
		string name;
		string fullName;
		string paramName;
	};
	static const ModDef Modulations[END];

	const char* getParamStringValue(unsigned int value);
	ModulationTypes getModulation() { return effect; }
	unsigned int getParameter() { return param; }
	bool getEnabled() { return on; }

	bool setModulation(ModulationTypes mod);
	bool setParameter(unsigned int p);
	void enable(bool flag) { on = flag; }

	const char* getParamName() const;
	unsigned int getMinParam();
	unsigned int getMaxParam();

	std::string toString() const;


private:
	ModulationTypes effect;
	unsigned int param;
	bool on;

	static const char* envelopeValues[];
	static const char* scaleValues[];
	static const char* pitchValues[];
	static const char* commonValues[];
	static const char* chorusValues[];

	ModulationTypes validateFx(ModulationTypes m);
	unsigned int validateParameter(unsigned int value);
};

#endif // PRESETMODULATION_H

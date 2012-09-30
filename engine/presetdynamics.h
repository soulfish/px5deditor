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
#ifndef PRESETDYNAMICS_H
#define PRESETDYNAMICS_H

#include <string>
using namespace std;

class PresetDynamics
{
public:

	enum DynamicsTypes {

		COMPRESSOR = 0,
		DYNAMICEXCITER,
		VOXWAH,
		WAHUP1,
		WAHUP2,
		WAHDOWN1,
		WAHDOWN2,
		SLOWATTACK,
		ACOUSTICSIM,
		RESONATOR,
		OCTAVE,
		RING,
		RINGUP,
		RINGDOWN,
		CRUSHER,
		HUM2SINGLE,
		SINGLE2HUM,
		HALFTONE,
		PHASEOUT,
		BASSCOMP,
		LIMITER,
		BASSOVERDRIVE,
		BASSDIST,
		BASSFUZZ,
		PREAMP1,
		PREAMP2,
		BALANCE,
		LOWCUT,
		FRETLESS,

		END
	};

	struct EffectDef {
		DynamicsTypes index;
		string name;
		string fullName;
		string parameterName;
	};
	static const EffectDef Effects[END];

	PresetDynamics();
	const char* getParamStringValue(unsigned int value);

	DynamicsTypes getEffect() { return effect; }
	unsigned int getParameter() { return parameter; }
	bool getEnabled() { return on; }

	bool setEffect(DynamicsTypes fx);
	bool setParameter(unsigned int p);
	void enable(bool flag) { on = flag; }

	const char* getParamName();
	unsigned int getMinParam();
	unsigned int getMaxParam();

private:
	DynamicsTypes effect;
	unsigned int parameter;
	bool on;

	static const char* commonValues[];
	DynamicsTypes validateFx(DynamicsTypes m);
	unsigned int validateParameter(unsigned int value);

};

#endif // PRESETDYNAMICS_H

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
#ifndef PRESETCABINET_H
#define PRESETCABINET_H

#include <string>

using namespace std;

class PresetCabinet
{
public:
	PresetCabinet();

	enum CabTypes {
		TWEED1x8,
		BLACK2x10,
		TWEED4x10,
		TWEED1x12,
		VOX1x12,
		BLACK2x12,
		VOX2x12,
		VOX4x12,
		CLS4x12,
		MDN4x12,
		VIN4x12,
		LA4x10,
		MDN4x10,
		MTL4x10,
		CLS8x10,
		UK4x12,
		STU1x15,
		JAZ1x15,
		AC2x15,
		US2x15,
		UK4x15,
		LA1x18,
		COMBI,

		END
	};

	struct CabDef {
		CabTypes index;
		string name;
		string fullName;
	};
	static const CabDef Cabs[END];

	const char* getParamStringValue(unsigned int value);
	CabTypes getCabinet() { return model; }
	unsigned int getParameter() { return presence; }
	bool getEnabled() { return on; }

	bool setCabinet(CabTypes cab);
	bool setParameter(unsigned int p);
	void enable(bool flag) { on = flag; }

	const char* getParamName();
	unsigned int getMinParam();
	unsigned int getMaxParam();

private:
	CabTypes model;
	unsigned int presence;
	bool on;

	static const char* commonValues[];
	CabTypes validateModel(CabTypes m);
	unsigned int validateParameter(unsigned int value);

};

#endif // PRESETCABINET_H

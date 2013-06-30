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
#ifndef PRESETREVERB_H
#define PRESETREVERB_H

#include <string>
using namespace std;

class PresetReverb
{
public:
	PresetReverb();

	enum ReverbTypes {
		DRYAIR,
		WETAIR,
		ROOM1,
		ROOM2,
		PLATE1,
		PLATE2,
		HALL1,
		HALL2,
		SPRING1,
		SPRING2,
		BRIGHT,
		END
	};

	struct ReverbDef {
		ReverbTypes index;
		string name;
		string fullName;
	};
	static const ReverbDef Reverbs[END];

	const char* getParamStringValue(unsigned int value);

	ReverbTypes getReverb() { return type; }
	unsigned int getParameter() { return fxLevel; }
	bool getEnabled() { return on; }

	bool setReverb(ReverbTypes fx);
	bool setParameter(unsigned int p);
	void enable(bool flag) { on = flag; }

	const char* getParamName() const;
	unsigned int getMinParam();
	unsigned int getMaxParam();

	std::string toString() const;

private:
	ReverbTypes type;
	unsigned int fxLevel;
	bool on;

	static const char* commonValues[];
	ReverbTypes validateFx(ReverbTypes m);
	unsigned int validateParameter(unsigned int value);

};

#endif // PRESETREVERB_H

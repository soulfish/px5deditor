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
#ifndef PRESETDELAY_H
#define PRESETDELAY_H

#include <string>
using namespace std;

class PresetDelay
{
public:
	PresetDelay();

	enum DelayTypes {
		SLAP1,
		SLAP2,
		SLAP3,
		SLAP4,
		SLAP5,
		ECHO1,
		ECHO2,
		ECHO3,
		ECHO4,
		ECHO5,
		CLRDLY1,
		CLRDLY2,
		CLRDLY3,
		CLRDLY4,
		CLRDLY5,
		PPONGDLY1,
		PPONGDLY2,
		PPONGDLY3,
		PPONGDLY4,
		PPONGDLY5,
		MULTITAP1,
		MULTITAP2,
		MULTITAP3,
		MULTITAP4,
		MULTITAP5,
		REVERSEDLY,
		END
	};

	struct DelayDef {
		DelayTypes index;
		string name;
		string fullName;
	};
	static const DelayDef Delays[END];

	const char* getParam1StringValue(unsigned int value);
	const char* getParam2StringValue(unsigned int value);

	DelayTypes getDelay() { return type; }
	unsigned int getParameter1() { return fxLevel; }
	unsigned int getParameter2() { return timeMS; }

	bool getEnabled() { return on; }

	bool setDelay(DelayTypes dly);
	bool setParameter1(unsigned int p);
	bool setParameter2(unsigned int p);

	void enable(bool flag) { on = flag; }

	const char* getParam1Name();
	const char* getParam2Name();

	unsigned int getMinParam1();
	unsigned int getMaxParam1();

	unsigned int getMinParam2();
	unsigned int getMaxParam2();

private:
	DelayTypes type;
	unsigned int fxLevel;
	unsigned int timeMS;
	bool on;

	static const char* commonValues[];
	DelayTypes validateFx(DelayTypes m);
	unsigned int validateParameter(unsigned int param, unsigned int value);

};

#endif // PRESETDELAY_H

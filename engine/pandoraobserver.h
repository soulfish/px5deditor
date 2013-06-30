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
#ifndef PANDORAOBSERVER_H
#define PANDORAOBSERVER_H

#include "presetparameters.h"
#include "pandorapreset.h"
#include <string>

using namespace std;

class PandoraObservable;

class PandoraObserver
{
public:
	PandoraObserver();
	virtual void update(PandoraObservable* o, PandoraUpdatedSignal bitflag, PandoraPreset &p) = 0;
	virtual void update(PandoraObservable* o, PandoraUpdatedSignal bitflag) = 0;
	virtual ~PandoraObserver() {}
};

#endif // PANDORAOBSERVER_H

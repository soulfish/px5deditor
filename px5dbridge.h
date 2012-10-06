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
#ifndef PX5DBRIDGE_H
#define PX5DBRIDGE_H

#include "engine/px5dcontroller.h"
#include "engine/pandoraobservable.h"
#include "engine/pandoraobserver.h"

#include <QObject>

class PX5DBridge : public PandoraObserver
{
public:
	PX5DBridge();
	~PX5DBridge();
	Px5dController *controller();
	bool initialize(QObject *parent);

	virtual void update(PandoraObservable* o, PandoraUpdatedSignal bitflag, PandoraPreset &p);

private:
	Px5dController *m_px5dController;
	QObject* m_parent;
};

#endif // PX5DBRIDGE_H

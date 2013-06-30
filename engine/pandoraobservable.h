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

#ifndef PANDORAOBSERVABLE_H
#define PANDORAOBSERVABLE_H

#include <set>
#include "presetparameters.h"
#include "pandorapreset.h"
#include "pandoraobserver.h"

class PandoraObservable
{
public:
	PandoraObservable();

   bool changed;
   std::set<PandoraObserver*> observers;

protected:
   virtual void setChanged() { changed = true; }
   virtual void clearChanged() { changed = false; }

public:
   virtual void addObserver(PandoraObserver& o) {
	   observers.insert(&o);
   }

   virtual void deleteObserver(PandoraObserver& o) {
		observers.erase(&o);
   }

   virtual void deleteObservers() {
		observers.clear();
   }

   virtual int countObservers() {
		return observers.size();
   }

   virtual bool hasChanged() { return changed; }

   // Full notification passing the preset object
   virtual void notifyObservers(PandoraUpdatedSignal bitflag, PandoraPreset &p) {
	  if(!hasChanged()) return;
		clearChanged();
		std::set<PandoraObserver*>::iterator it;
		for(it = observers.begin();it != observers.end(); it++) {
			(*it)->update(this, bitflag, p);
		}
   }

   // Event notification
   virtual void notifyObservers(PandoraUpdatedSignal bitflag) {
	  if(!hasChanged()) return;
		clearChanged();
		std::set<PandoraObserver*>::iterator it;
		for(it = observers.begin();it != observers.end(); it++) {
			(*it)->update(this, bitflag);
		}
   }

   virtual ~PandoraObservable() {}
};

#endif // PANDORAOBSERVABLE_H

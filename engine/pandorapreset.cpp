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

#include "pandorapreset.h"
#include <string>
#include <iostream>

PandoraPreset::PandoraPreset()
{
	number = 0;
	slot = PROGRAM_USER;
}

PandoraPreset::PandoraPreset(const PandoraPreset& orig) {

	slot = orig.slot;
	number = orig.number;
	name = orig.name;

	dynamics = orig.dynamics;
	amp = orig.amp;
	cabinet = orig.cabinet;
	delay = orig.delay;
	modulation = orig.modulation;
	noiseReduction = orig.noiseReduction;
	reverb = orig.reverb;
}

PandoraPreset::~PandoraPreset() {

}

void PandoraPreset::print(const std::string& msg) const {

	if ( msg.size() != 0 ) {
		std::cout << msg << std::endl;
	}

	std::cout << "\t" << "Program #" << number << " (" << ((slot==PROGRAM_FACTORY)?"P":"U") << ") - '" << name.toString() << " '" << std::endl;

	std::cout << "\t\t Dynamics   : " << dynamics.toString() << std::endl;
	std::cout << "\t\t Amp        : " << amp.toString() << std::endl;
	std::cout << "\t\t Cabinet    : " << cabinet.toString() << std::endl;
	std::cout << "\t\t Modulation : " << modulation.toString() << std::endl;
	std::cout << "\t\t Delay      : " << delay.toString() << std::endl;
	std::cout << "\t\t Reverb     : " << reverb.toString() << std::endl;
	std::cout << "\t\t NoiseReduc : " << noiseReduction.toString() << std::endl;
}

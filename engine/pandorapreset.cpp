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

PandoraPreset::PandoraPreset()
{
	name = "";
	number = 0;
	slot = PROGRAM_USER;
}

PandoraPreset::PandoraPreset(const PandoraPreset& orig) {

}

PandoraPreset::~PandoraPreset() {

}

/*
 * Translate a pandora character to an ascii character
 */
char PandoraPreset::translatePandoraCharacter( const char px5char ) {
	const std::string charTranslation = " /\"f$n&'><*+,-|/0123456789_,{=}?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]~";

	// I could not find equivalent of these characters, I replaced them with similar:
	// TODO: check out ASCII map
	// 0x01 : broken /, mix of a low pipe and upper slash
	// 0x03 : barred F
	// 0x05 : sort of reversed N
	// 0x0E : bottom half of |
	// 0x1A : _ with overline
	// 0x1B : , with overline
	// 0x1C : strange triangle/arrow left
	// 0x1E : strange triangle/arrow right
	// 0x3E : ~ weird

	//FIXME: Assert we're inside the translation string
	if ( px5char < charTranslation.length() ) {
		return charTranslation.at(px5char);
	} else {
		return '?';
	}
}

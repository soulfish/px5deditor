#include "presetname.h"

PresetName::PresetName() {
	m_name = "UNKNOWN";
}

/*
 * Convert a character code of the Pandora to an ASCII character *
 * Pandora has 63 allowed characters
 * From 0x0 to 0x3E they map to the standard ASCII table starting at 0x20 (space) to  0x5E (^)
 */
char PresetName::translatePandoraCharacterToAscii( const char px5char ) {
	if ( int(px5char)>=0 && int(px5char)<=0x3E ) {
		return char( int(px5char) + ' ' );
	} else {
		return '?';
	}
}

/*
 * Convert an ASCII character to a Pandora character
 */
char PresetName::translateAsciiCharacterToPandora( const char asciichar ) {
	if ( asciichar>=' ' && asciichar<='~' ) {
		return char( asciichar - ' ' );
	} else {
		return '?';
	}
}

void PresetName::setFromSysex(const std::vector<unsigned char>& sysexName) {
	m_name = "";
	for ( int i=0; i<sysexName.size(); i++ ) {
		m_name += translatePandoraCharacterToAscii(sysexName.at(i));
	}
}

void PresetName::setFromString(const char* name) {

	//TODO: test for unallowed characters!
	m_name = name;
	if (m_name.length()>maxNameSize()) {
		m_name = m_name.substr(0, maxNameSize());
	}
}

void PresetName::setLetterInName(unsigned int position, const char asciiChar) {

	if ( position >= maxNameSize() ) {
		return;
	}

	if ( position>=m_name.length() ) {
		//NOTE: This should not happen but we might
		//need to handle that
		return;
	}

	m_name.at(position) = asciiChar;
}

unsigned int PresetName::maxNameSize() {
	// PX5D has 7 characters max in program names
	return 7;
}

char PresetName::minNameAsciiChar() {
	return ' ';
}

char PresetName::maxNameAsciiChar() {
	return '^';
}

std::string PresetName::toString() const {
	return m_name;
}


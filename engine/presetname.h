#ifndef PRESETNAME_H
#define PRESETNAME_H

#include <string>
#include <vector>

class PresetName
{
public:
	PresetName();

	void setFromSysex(const std::vector<unsigned char>& sysexName);
	void setFromString(const char* name);

	const std::string getAsciiName() { return m_name; }

	void setLetterInName(unsigned int position, const char asciiChar);

	static char translatePandoraCharacterToAscii( const char px5char );
	static char translateAsciiCharacterToPandora( const char asciichar );
	static unsigned int maxNameSize();
	static char minNameAsciiChar();
	static char maxNameAsciiChar();

	std::string toString() const;

private:
	std::string m_name;
};

#endif // PRESETNAME_H

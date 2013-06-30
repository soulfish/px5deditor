#ifndef PRESETREADER_H
#define PRESETREADER_H

#include <QString>
#include "pandorapreset.h"

class PresetReader {
public:

	enum FileType {
		UNKNOWN,
		SINGLE,
		MULTI
	};


	enum PresetsEnableFlags {
		SWITCH_NONE = 0x0,
		SWITCH_DYN = 0x1,
		SWITCH_AMP = 0x2,
		SWITCH_CAB = 0x4,
		SWITCH_MOD = 0x8,
		SWITCH_DELAY = 0x30,
		SWITCH_REVERB = 0x40
	};

    PresetReader();
	bool readPreset(const QString &presetFile);
	FileType identifyType(QDataStream &in, quint32 &count);
	void parsePreset(QDataStream &in, PandoraPreset &p);

	unsigned int presetCount() { return m_presetCount; }
	PandoraPreset &getPreset(unsigned int i);

private:
	std::vector<PandoraPreset> m_presets;
	unsigned int m_presetCount;
};

#endif // PRESETREADER_H

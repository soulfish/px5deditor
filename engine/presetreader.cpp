#include "presetreader.h"

#include <QFile>
#include <QDataStream>
#include <QString>
#include <QDebug>

#include "pandorapreset.h"
#include "presetname.h"

/*
 File preset reader

 Reads a preset from the original windows app and
 fills in our structsers.
*/

PresetReader::PresetReader() {
	m_presetCount = 0;
}

PandoraPreset &PresetReader::getPreset(unsigned int i) {
	//FIXME: handle out of bound index
	return m_presets.at(i);
}

//FIXME: refactor name to plural
bool PresetReader::readPreset(const QString &presetFile) {

	m_presets.clear();

	QFile file(presetFile);

	if ( ! file.open(QIODevice::ReadOnly) ) {
		qDebug("Preset file note found!");
		return false;
	}

	QDataStream in(&file);

	quint32 presetsCount = 0;
	FileType pt = this->identifyType(in, presetsCount);

	m_presetCount = presetsCount;

	if ( pt==PresetReader::SINGLE || pt==PresetReader::MULTI ) {
		qDebug() << "Parsing " << presetsCount << " presets... ";

		for ( int i=0; i<presetsCount; ++i ){
			PandoraPreset p;
			this->parsePreset(in, p);
			p.print("Parsed Preset: ");
			m_presets.push_back(p);
		}
	}
	file.close();
	return true;
}

PresetReader::FileType PresetReader::identifyType(QDataStream &in, quint32 &count) {

	// read common block
	uchar tb[16];
	in.readRawData((char*)tb, 16);
	if ( tb[0]!=0x10 || tb[1]!=0x45 ) {
		qDebug() << "Common header invalid, found " << tb[0] << " / " << tb[1];
		return PresetReader::UNKNOWN;
	}

	// read preset def block
	in.readRawData((char*)tb, 16);
	if ( tb[0]!=0x20 ) {
		qDebug() << "Type header invalid, found " << tb[0];
		return PresetReader::UNKNOWN;
	}
	quint32 presetsCount = tb[3];
	count = presetsCount;
	qDebug() << " Found presets: " << presetsCount;
	if ( presetsCount==1 ) {
		return PresetReader::SINGLE;
	} else if ( presetsCount>0 ) {
		return PresetReader::MULTI;
	}

	return PresetReader::UNKNOWN;
}

void PresetReader::parsePreset(QDataStream &in, PandoraPreset &p) {

	// total preset size is 58 (16+42) bytes
	char name[8];
	in.readRawData(name, 7);
	name[7]=0;

	for ( int i=0; i<7; ++i ) {
		name[i] = PresetName::translatePandoraCharacterToAscii(name[i]);
	}
	qDebug() << " Preset Name: " << name;

	p.name.setFromString(name);
	quint8 effectsBitflag;
	in >> effectsBitflag;

	p.amp.enable( effectsBitflag && PresetReader::SWITCH_AMP );
	p.cabinet.enable( effectsBitflag && PresetReader::SWITCH_CAB );
	p.delay.enable( effectsBitflag && PresetReader::SWITCH_DELAY );
	p.dynamics.enable( effectsBitflag && PresetReader::SWITCH_DYN );
	p.modulation.enable( effectsBitflag && PresetReader::SWITCH_MOD );
	p.reverb.enable( effectsBitflag && PresetReader::SWITCH_REVERB );

	quint8 dynType;
	in >> dynType;
	p.dynamics.setEffect((PresetDynamics::DynamicsTypes)dynType);

	quint8 ampModel;
	in >> ampModel;
	p.amp.setModel((PresetAmp::AmpTypes)ampModel);

	quint8 cabModel;
	in >> cabModel;
	p.cabinet.setCabinet((PresetCabinet::CabTypes)cabModel);

	quint8 modType;
	in >> modType;
	p.modulation.setModulation((PresetModulation::ModulationTypes)modType);

	quint8 delayType;
	in >> delayType;
	p.delay.setDelay((PresetDelay::DelayTypes)delayType);

	quint8 reverbType;
	in >> reverbType;
	p.reverb.setReverb((PresetReverb::ReverbTypes)reverbType);

	quint8 dynValue;
	in >> dynValue;
	p.dynamics.setParameter(dynValue);

	quint8 eqMiddle;
	in >> eqMiddle;
	p.amp.setMiddle(eqMiddle);

	quint8 eqPresence;
	in >> eqPresence;
	p.amp.setMiddle(eqPresence);

	quint8 modValue;
	in >> modValue;
	p.modulation.setParameter(modValue);

	quint8 delayFX;
	in >> delayFX;
	p.delay.setParameter1(delayFX);

	quint8 delayTime;
	in >> delayTime;
	p.delay.setParameter2(delayTime);

	quint8 reverbLevel;
	in >> reverbLevel;
	p.reverb.setParameter(reverbLevel);

	quint8 ampGain;
	in >> ampGain;
	p.amp.setGain(ampGain);

	quint8 ampBass;
	in >> ampBass;
	p.amp.setBass(ampBass);

	quint8 ampTreble;
	in >> ampTreble;
	p.amp.setTreble(ampTreble);

	quint8 ampVolume;
	in >> ampVolume;
	p.amp.setVolume(ampVolume);

	quint8 noiseReduction;
	in >> noiseReduction;
	p.noiseReduction.setParameter(noiseReduction);

	/*
	static final int SWITCH_OFF = 0x1;
	static final int SWITCH_DYN = 0x1;
	static final int SWITCH_AMP = 0x2;
	static final int SWITCH_CAB = 0x4;
	static final int SWITCH_MOD = 0x8;
	static final int SWITCH_DELAY = 0x30;
	static final int SWITCH_REVERB = 0x40;

	static final int SINGLE_PRESET_OFFSET = 0x27;

static final int PRESET_NAME_OFFSET = 0x0; // runs for 7 bytes
static final int PRESET_NAME_SIZE = 7; // runs for 7 bytes
static final int EFFECTS_BITFLAG = 0x07;
static final int DYN_TYPE = 0x08;// each on one byte
static final int AMP_MODEL = 0x09;
static final int CAB_MODEL = 0x0A;
static final int MOD_TYPE = 0x0B;
static final int DELAY_TYPE = 0x0C;
static final int REVERB_TYPE = 0x0D;
static final int DYN_VALUE = 0x0E;
static final int EQ_MIDDLE = 0x0F;
static final int EQ_PRESENCE = 0x10;
static final int MOD_VALUE = 0x11;
static final int DELAY_FX = 0x12;
static final int DELAY_TIME = 0x13;
static final int REVERB_LEVEL = 0x14;
static final int AMP_GAIN = 0x15;
static final int AMP_BASS = 0x16;
static final int AMP_TREBLE = 0x17;
static final int AMP_VOLUME = 0x18;
static final int NOISE_REDUCTION = 0x19;
static final int SWITCH_OFF = 0x1;
static final int SWITCH_DYN = 0x1;
static final int SWITCH_AMP = 0x2;
static final int SWITCH_CAB = 0x4;
static final int SWITCH_MOD = 0x8;
static final int SWITCH_DELAY = 0x30;
static final int SWITCH_REVERB = 0x40;

	static final int PRESET_BYTE_SIZE = 26;


	private SoundPreset readPreset(byte[] buff) {

		SoundPreset np = new SoundPreset();
		np.presetName = "";
		for ( int i=0; i<PRESET_NAME_SIZE; ++i ) {
			np.presetName += translatePandoraCharacter( buff[i] );
		}
		System.out.println( "Read preset: " + np.presetName + "\n");

		int switchGrid = buff[EFFECTS_BITFLAG];

		np.amp.on = (switchGrid & SWITCH_AMP)!=0;
		np.amp.bass = buff[AMP_BASS];
		np.amp.gain = buff[AMP_GAIN];
		np.amp.middle = buff[AMP_GAIN];
		np.amp.treble = buff[AMP_TREBLE];
		np.amp.model = buff[AMP_MODEL];
		np.amp.volume = buff[AMP_VOLUME];

		np.dynamics.on = (switchGrid & SWITCH_DYN)!=0;
		np.dynamics.effect = buff[DYN_TYPE];
		np.dynamics.sensitivity = buff[DYN_VALUE];

		np.cabinet.on = (switchGrid & SWITCH_CAB)!=0;
		np.cabinet.model = buff[CAB_MODEL];
		np.cabinet.presence = buff[EQ_PRESENCE];

		np.modulation.on = (switchGrid & SWITCH_MOD)!=0;
		np.modulation.effect = buff[MOD_TYPE];
		np.modulation.speed = buff[MOD_VALUE];

		np.delay.on = (switchGrid & SWITCH_DELAY)!=0;
		np.delay.type = buff[DELAY_TYPE];
		np.delay.fxLevel = buff[DELAY_FX];
		np.delay.timeMS = buff[DELAY_TIME];

		np.reverb.on = (switchGrid & SWITCH_REVERB)!=0;
		np.reverb.fxLevel = buff[REVERB_LEVEL];
		np.reverb.type = buff[REVERB_TYPE];

		np.noiseReduction.noiseReductionLevel = buff[NOISE_REDUCTION];
		np.noiseReduction.on = (np.noiseReduction.noiseReductionLevel>0);


		return np;
	}

*/
}
/*
package pandora;


import pandora.preset.SoundPreset;

public class PandoraPresetFileReader {



	enum PresetFileType { UNKNOWN, SINGLE_PRESET, MULTI_PRESETS, CHAIN_PRESETS  };


	List<SoundPreset> allPresets;


	public PandoraPresetFileReader(String presetFile) throws Exception {

		// Must reset presets
		allPresets = new ArrayList<SoundPreset>();

		File f = new File(presetFile);
		if ( f.exists() ) {
			InputStream is = new FileInputStream(f);
			PresetFileType filetype = identifyFileType(is);
			if ( filetype == PresetFileType.UNKNOWN ) {
				throw new Exception("Unknown File Type");
			} else if ( filetype == PresetFileType.SINGLE_PRESET ) {

				System.out.println("Reading Single Preset...\n");
				byte[] buff = new byte[PRESET_BYTE_SIZE];
				is.read(buff);
				SoundPreset pr = readPreset(buff);
				allPresets.add(pr);

			} else if ( filetype == PresetFileType.MULTI_PRESETS ) {
				System.out.println("Reading Multiple Presets...\n");

				byte[] buff = new byte[PRESET_BYTE_SIZE];
				while ( is.read(buff)==PRESET_BYTE_SIZE ) {
					SoundPreset pr = readPreset(buff);
					allPresets.add(pr);
				}

			}

		}
		else {
			System.err.println("File " + presetFile + " not found");
		}
	}

	private PresetFileType identifyFileType(InputStream is) throws IOException {
		PresetFileType pft = PresetFileType.UNKNOWN;
		// read common block
		byte[] buff = new byte[commonFileHeader.length];
		is.read(buff);
		for (int i=0; i<buff.length; ++i ) {
			if ( buff[i]!=commonFileHeader[i] ) {
				return PresetFileType.UNKNOWN;
			}
		}

		buff = new byte[singlePresetFileHeader.length];
		is.read(buff);
		boolean allGood = false;
		for (int i=0; i<buff.length; ++i ) {
			if ( buff[i]!=singlePresetFileHeader[i] ) {
				allGood = false;
				break;
			} else {
				allGood = true;
			}
		}
		if ( allGood ) {
			return PresetFileType.SINGLE_PRESET;
		}

		allGood = false;
		for (int i=0; i<buff.length; ++i ) {
			if ( buff[i]!=multiPresetsFileHeader[i] ) {
				allGood = false;
				break;
			} else {
				allGood = true;
			}
		}

		if ( allGood ) {
			return PresetFileType.MULTI_PRESETS;
		} else {
			return PresetFileType.UNKNOWN;
		}
	}

	public void dump() {
		Iterator<SoundPreset> it = allPresets.iterator();
		do {
			SoundPreset preset = it.next();
			System.out.println( preset.toString() + "\n");
		} while ( it.hasNext() );

	}

	public char translatePandoraCharacter( byte px5char ) {
		final String charTranslation = " /\"f$n&'><*+,-|/0123456789_,{=}?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]~";
		// could not find equivalent of these characters:
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

		try {
			return charTranslation.charAt(px5char);
		} catch (Exception e) {
			return '?';
		}
	}
}

*/

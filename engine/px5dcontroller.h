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

#ifndef PX5DCONTROLLER_H
#define PX5DCONTROLLER_H

#include "RtMidi.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "pandoraobservable.h"
#include "pandorapreset.h"
#include "presetparameters.h"

using namespace std;

class Px5dController : public PandoraObservable
{
public:
	Px5dController();
	~Px5dController();

	bool connectToPandora();
	bool isConnected() { return m_isConnected; }

	string pandoraInPortName() { return m_pandoraInPortName; }
	int pandoraInPort() { return m_pandoraInPort; }

	string pandoraOutPortName() { return m_pandoraOutPortName; }
	int pandoraOutPort() { return m_pandoraOutPort; }

	// Pandora control
	void setProgramNumber(unsigned int program, PandoraPreset::ProgramBank bank);

	void decodeSysex(std::vector< unsigned char > *message );

	void requestIdentity();
	void requestCurrentProgram();
	void requestFullDump();
	void requestCurrentProgramData();

	void setModuleState( PandoraNotification p, int value );
	void setParamChanged( PandoraNotification p, int value);

	PandoraPreset* Preset() { return &m_currentPreset; }

	void SaveCurrentProgramToSlot(int slot);

private:
	int m_pandoraInPort;
	RtMidiIn *m_midiIn;
	string m_pandoraInPortName;

	int m_pandoraOutPort;
	RtMidiOut *m_midiOut;
	string m_pandoraOutPortName;

	bool m_isConnected;
	string m_pandoraPx5dIdentifier;

	unsigned int m_currentProgram;
	void cleanup();

	enum tState { STATE_WAITING,
				  STATE_HANDSHAKE_PENDING,
				  STATE_CURRENT_PROGRAM_PENDING,
				  STATE_SINGLE_DUMP_PENDING,
				  STATE_ALL_DUMP_PENDING } ;

	bool m_pandoraHandshaked;
	tState m_pandoraMidiState;

	PandoraPreset m_currentPreset;

	void fillPandoraSysexHeader(std::vector< unsigned char > *message );

	void sendSysex(std::vector< unsigned char > *message, std::string debug );

	// These enums define the offset of the different
	// values inside the sysex program chunk sent by the pandora.
	enum PandoraPresetOffsets {

		PX5D_SYSEX_COMMAND = 0x05, // command
		PX5D_SYSEX_NAME_START = 0x07, // first letter of name
		PX5D_SYSEX_NAME_END = 0x0e, // las letter of name

		PX5D_SYSEX_EFFECTS_BITFLAG = 0x0f, // Bitflag representing enabled modules

		PX5D_SYSEX_DYN_TYPE = 0x10, // Dynamic effect type
		PX5D_SYSEX_AMP_MODEL = 0x11, // Amp Model
		PX5D_SYSEX_CAB_TYPE = 0x12, // Cabinet Model
		PX5D_SYSEX_MOD_TYPE = 0x13, // Modulation type
		PX5D_SYSEX_DELAY_TYPE = 0x14, // Delay type
		PX5D_SYSEX_REVERB_TYPE = 0x15, // Reverb type

		PX5D_SYSEX_DYN_VALUE = 0x17, // Dynamic effect parameter
		PX5D_SYSEX_MIDDLE = 0x18, // Middle
		PX5D_SYSEX_CAB_VALUE = 0x19, // Cab param
		PX5D_SYSEX_MOD_VALUE = 0x1a, // Modulation param
		PX5D_SYSEX_DELAY_VALUE = 0x1b, // Delay param
		PX5D_SYSEX_DELAY_TIME_VALUE = 0x1c, // Delay time
		PX5D_SYSEX_REVERB_VALUE = 0x1d, // Reverb param

		PX5D_SYSEX_GAIN = 0x1f, // Gain
		PX5D_SYSEX_BASS = 0x20, // Bass
		PX5D_SYSEX_TREBLE = 0x21, // Treble
		PX5D_SYSEX_VOLUME = 0x22, // Volume
		PX5D_SYSEX_NOISE_REDUCTION = 0x23 // Noise reduction

	};

	enum PandoraPresetsEnableFlags {
		PX5D_SYSEX_SWITCH_OFF = 0x0,
		PX5D_SYSEX_SWITCH_DYN = 0x1,
		PX5D_SYSEX_SWITCH_AMP = 0x2,
		PX5D_SYSEX_SWITCH_CAB = 0x4,
		PX5D_SYSEX_SWITCH_MOD = 0x8,
		PX5D_SYSEX_SWITCH_DELAY = 0x30,
		PX5D_SYSEX_SWITCH_REVERB = 0x40
	};

};

#endif // PX5DCONTROLLER_H

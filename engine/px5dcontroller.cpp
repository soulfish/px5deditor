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

#include <iomanip>
#include "px5dcontroller.h"
#include "RtError.h"
#include <signal.h>
#include "presetparameters.h"
#include "pandorapreset.h"

// FIXME !!! MAKE IT THREAD SAFE !!!!

// Platform-dependent sleep routines.
#if defined(__WINDOWS_MM__)
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants
#include <unistd.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

//TODO: make Px5dController a singleton
// http://www.yolinux.com/TUTORIALS/C++Singleton.html

/*
 * midi callback function for RtMidi
 **/
void midiInCallback( double deltatime, std::vector< unsigned char > *message, void *userData ) {

	Px5dController* instance = static_cast<Px5dController*>(userData);

#ifdef DEBUG
	unsigned int nBytes = message->size();
	std::cout << "[@" << deltatime << "]  " << std::endl;
	std::cout << "MIDI IN: " ;
	std::cout << setfill ('0') << setw(2);
	for ( unsigned int i=0; i<nBytes; i++ )
		std::cout << setfill ('0') << setw(2) << hex << (int)message->at(i) << " ";
	std::cout << std::endl;
#endif

	instance->decodeSysex( message );

}

/* Constructor
 **/
Px5dController::Px5dController() {
	m_midiIn = 0;
	m_midiOut = 0;
	m_pandoraPx5dIdentifier = "PANDORA PX5D";
	m_pandoraMidiState = STATE_WAITING;
	cleanup();
}

/* Destructor  */
Px5dController::~Px5dController() {
	cleanup();
}

void Px5dController::cleanup() {
	if ( m_midiIn!=0 ) {
		delete m_midiIn;
	}
	m_midiIn = 0;
	m_pandoraInPort = -1;
	m_pandoraInPortName = "";

	if ( m_midiOut!=0 ) {
		delete m_midiOut;
	}
	m_midiOut = 0;
	m_pandoraOutPort = -1;
	m_pandoraOutPortName = "";

	m_currentProgram = 0;
	m_isConnected = false;
	m_pandoraHandshaked = false;
}

/* Fill the mesage with the common pandora index */
void Px5dController::fillPandoraSysexHeader(std::vector< unsigned char > *message)
{
	// common sysex header
	message->push_back( 0xf0 );
	message->push_back( 0x42 );
	message->push_back( 0x30 );
	message->push_back( 0x73 );
	message->push_back( 0x20 );
}

/*
 * Look for pandora and connect
 **/
bool Px5dController::connectToPandora() {

	// Setup Midi In (receive from Pandora)
	m_midiIn = new RtMidiIn();
	unsigned int nPorts = m_midiIn->getPortCount();
	if ( nPorts==0 ) {
		std::cout << "No input ports available!\n";
		cleanup();
		return false;
	}

	for ( unsigned int p=0; p<nPorts; p++ ) {
		std::string portName = m_midiIn->getPortName(p);
		std::cout << "Found Input port: " << portName << std::endl;
		if ( portName.substr(0, m_pandoraPx5dIdentifier.length()).compare(m_pandoraPx5dIdentifier)==0 ) {
			m_pandoraInPortName = portName;
			m_pandoraInPort = p;
			break;
		}
	}
	if ( m_pandoraInPort==-1 ) {
		std::cout << "Pandora Input not found!\n";
		cleanup();
		return false;
	}

	m_midiIn->openPort(m_pandoraInPort);
	m_midiIn->ignoreTypes(false, true, true);

	// Setup Midi Out (send to Pandora)
	m_midiOut = new RtMidiOut();
	nPorts = m_midiOut->getPortCount();
	if ( nPorts==0 ) {
		std::cout << "No output ports available!\n";
		cleanup();
		return false;
	}

	for ( unsigned int p=0; p<nPorts; p++ ) {
		std::string portName = m_midiOut->getPortName(p);
		std::cout << "Found output port: " << portName << std::endl;
		if ( portName.substr(0, m_pandoraPx5dIdentifier.length()).compare(m_pandoraPx5dIdentifier)==0 ) {
			m_pandoraOutPortName = portName;
			m_pandoraOutPort = p;
			break;
		}
	}
	if ( m_pandoraOutPort==-1 ) {
		std::cout << "Pandora Output not found!\n";
		cleanup();
		return false;
	}
	m_midiOut->openPort(m_pandoraOutPort);
	m_midiIn->setCallback( &midiInCallback, this );
	m_isConnected = true;

	requestIdentity();
	requestCurrentProgram();
	requestCurrentProgramData();

	return true;
}

/*
 * Ask pandora to change program program change
 **/
void Px5dController::setProgramNumber(unsigned int program, PandoraPreset::ProgramBank bank) {

	if ( !m_isConnected ) return;

	if ( m_currentPreset.number==program && m_currentPreset.slot==bank ) {

#ifdef DEBUG
		std::cout << "Program is already " << program << " in same bank" << std::endl;
#endif
		return;
	}

	std::vector<unsigned char> message;

	// PROGRAM CHANGE DEF:	f0 42 30 73 20 4e 10 NN f7
	// PROGRAM CHANGE USER: f0 42 30 73 20 4e 00 NN f7

	// common sysex header
	fillPandoraSysexHeader(&message);

	// specific message
	message.push_back( 0x4e );

	if ( bank==PandoraPreset::PROGRAM_FACTORY ) {
		message.push_back( 0x10 );
	} else { // USER
		message.push_back( 0x00 );
	}

	message.push_back( program );

	// Sysex close
	message.push_back( 0xf7 );

	sendSysex( &message, "change program" );

	m_currentPreset.number=program;
	m_currentPreset.slot=bank;

	requestCurrentProgramData();
}

/*
 * Decode any sysex sent by Pandora
 **/
void Px5dController::decodeSysex(std::vector< unsigned char > *sysex )
{
	if ( !m_isConnected ) return;

#ifdef DEBUG
	std::cout << "Decoding SysEx from Pandora... " << std::endl;
#endif
	// TODO: pandora sends f0 42 30 73 20 23 f7
	// after a program change request ... why ?
	// and other commands : is it a "operation success" reply ?

	//TO Implement:
	// Enter Edit Mode: f0 42 30 73 20 4e 43 0b f7
	// Exit Edit Mode: f0 42 30 73 20 4e 40 0b f7

	// Sysex identity response: F0 7E 00 06 02 42 73 00 20 00 00 00 01 00 F7
	if ( sysex->at(0)==0xf0 &&
		 sysex->at(1)==0x7e &&
		 sysex->at(1)==0x00 &&
		 sysex->at(1)==0x06 &&
		 sysex->at(1)==0x02 &&
		 sysex->at(1)==0x42 &&
		 sysex->at(1)==0x73 &&
		 sysex->at(1)==0x00 &&
		 sysex->at(1)==0x20 &&
		 sysex->at(1)==0x00 &&
		 sysex->at(1)==0x00 &&
		 sysex->at(1)==0x00 &&
		 sysex->at(1)==0x01 &&
		 sysex->at(1)==0x00 &&
		 sysex->at(1)==0xf7 ) {

		m_pandoraHandshaked = true;

		// FIXME: is it used anywhere ?
		setChanged();
		notifyObservers(PX5D_HANDSHAKE, m_currentPreset);

	}

	// all pandora commands start with: f0 42 30 73 20
	else if ( sysex->at(0)==0xf0 &&
			  sysex->at(1)==0x42 &&
			  sysex->at(2)==0x30 &&
			  sysex->at(3)==0x73 &&
			  sysex->at(4)==0x20 ) {


		// CURRENT PROGRAM QUERY answer
		// Sent from the Pandora after we asked the current program number

		// (f0 42 30 73 20) 42 50 XX 0f 00 f7 (default P)
		// (f0 42 30 73 20) 42 40 XX 0f 00 f7 (user U)
		if ( sysex->at(5)==0x42 &&
			 (sysex->at(6)==0x40 || sysex->at(6)==0x50 ) &&
			 sysex->at(8)==0x0f &&
			 sysex->at(9)==0x00 &&
			 sysex->at(10)==0xf7
			 ) {

			int programNumber = sysex->at(7);
			if ( sysex->at(6)==0x50 ) {
				// Factory/P program
				m_currentPreset.number = programNumber;
				m_currentPreset.slot = PandoraPreset::PROGRAM_FACTORY;

			} else if ( sysex->at(6)==0x40 ) {
				// User program
				m_currentPreset.number = programNumber;
				m_currentPreset.slot = PandoraPreset::PROGRAM_USER;
			}
		}

		// Current program data
		// Sent from the Pandora after we asked the current program data
		else if ( sysex->at(5)==0x40 && sysex->at(6)==0x00 ) {

#ifdef DEBUG
			std::cout << "PANDORA DEBUG PROGRAM SYSEX: " << std::endl;
			for ( unsigned int i=0; i<sysex->size(); ++i ) {
				std::cout << "[0x" << setfill('0') << setw(2) << hex << i << "] = 0x" << setfill('0') << setw(2) << hex << (int)sysex->at(i) << std::endl;
			}
#endif

			/* Decomposed:
			[0-6] (f0 42 30 73 20) 40 00 (sysex signature "sending current program")
					[7-N] 2d 25 34 21 2c 29 23 00 4f 01 0b 08 00 0a 02 00 15 01 15 0a 0f 03 09 00 1e 18 11 13 0c f7
					[7-N] m  e  t  a  l  i  c  00
												  4f 01 0b 08 00 0a 02 00 15 01 15 0a 0f 03 09 00
																								  1e 18 11 13 0c
																												 f7
					which is identical to what is stored inside the preset files, except the ending 00 which are note store in files.
			*/

			//TODO: pass the message to a utility function "extractName"
			string prgName = "";
			for ( int i=7; i<7+7; i++ ) {
				char c = sysex->at(i); //BUG!!
				prgName += PandoraPreset::translatePandoraCharacter( c );
			}

			m_currentPreset.name = prgName;

			// Decode preset parameters
			//TODO: pass the message to a utility function "extractPresetSettings"
			//NOT: offset + 8 car il faut compter le prefix sysex + le 00 après le nom (blocs de 7 + 1)
			int enabledModules = sysex->at(PX5D_SYSEX_EFFECTS_BITFLAG);

#ifdef DEBUG
			std::cout << "Modules flag: " << enabledModules << std::endl;
#endif
			bool dynOn = enabledModules & PX5D_SYSEX_SWITCH_DYN;
			bool ampOn = enabledModules & PX5D_SYSEX_SWITCH_AMP;
			bool cabOn = enabledModules & PX5D_SYSEX_SWITCH_CAB;
			bool modOn = enabledModules & PX5D_SYSEX_SWITCH_MOD;
			bool delayOn = enabledModules & PX5D_SYSEX_SWITCH_DELAY;
			bool reverbOn = enabledModules & PX5D_SYSEX_SWITCH_REVERB;

			m_currentPreset.dynamics.enable(dynOn);
			m_currentPreset.amp.enable(ampOn);
			m_currentPreset.cabinet.enable(cabOn);
			m_currentPreset.modulation.enable(modOn);
			m_currentPreset.delay.enable(delayOn);
			m_currentPreset.reverb.enable(reverbOn);

			int dynamicsModel = sysex->at(PX5D_SYSEX_DYN_TYPE);
			int dynamicsParam = sysex->at(PX5D_SYSEX_DYN_VALUE);

#ifdef DEBUG
			std::cout << "Got dyn model : " << dynamicsModel << std::endl;
			std::cout << "Got dyn param : " << dynamicsParam << std::endl;
#endif

			m_currentPreset.dynamics.setEffect((PresetDynamics::DynamicsTypes)dynamicsModel);
			m_currentPreset.dynamics.setParameter(dynamicsParam);

			m_currentPreset.amp.setModel((PresetAmp::AmpTypes)sysex->at(PX5D_SYSEX_AMP_MODEL));
			m_currentPreset.amp.setMiddle(sysex->at(PX5D_SYSEX_MIDDLE));
			m_currentPreset.amp.setBass(sysex->at(PX5D_SYSEX_BASS));
			m_currentPreset.amp.setTreble(sysex->at(PX5D_SYSEX_TREBLE));
			m_currentPreset.amp.setGain(sysex->at(PX5D_SYSEX_GAIN));
			m_currentPreset.amp.setVolume(sysex->at(PX5D_SYSEX_VOLUME));

			m_currentPreset.cabinet.setCabinet((PresetCabinet::CabTypes)sysex->at(PX5D_SYSEX_CAB_TYPE));
			m_currentPreset.cabinet.setParameter(sysex->at(PX5D_SYSEX_CAB_VALUE));

			m_currentPreset.modulation.setModulation((PresetModulation::ModulationTypes)sysex->at(PX5D_SYSEX_MOD_TYPE));
			m_currentPreset.modulation.setParameter(sysex->at(PX5D_SYSEX_CAB_VALUE));

			m_currentPreset.delay.setDelay((PresetDelay::DelayTypes)sysex->at(PX5D_SYSEX_DELAY_TYPE));
			m_currentPreset.delay.setParameter1(sysex->at(PX5D_SYSEX_DELAY_VALUE));
			m_currentPreset.delay.setParameter2(sysex->at(PX5D_SYSEX_DELAY_TIME_VALUE));

			m_currentPreset.reverb.setReverb((PresetReverb::ReverbTypes)sysex->at(PX5D_SYSEX_REVERB_TYPE));
			m_currentPreset.reverb.setParameter(sysex->at(PX5D_SYSEX_REVERB_VALUE));

			int noiseReductionLevel = sysex->at(PX5D_SYSEX_NOISE_REDUCTION);
			m_currentPreset.noiseReduction.setParameter(noiseReductionLevel);

			setChanged();
			notifyObservers(PX5D_UPDATE_ALL, m_currentPreset);

		}

		// Program change message
		// Sent from the Pandora when changing program number manually

		// PROGRAM CHANGE DEF:	(f0 42 30 73 20) 4e 10 NN f7
		// PROGRAM CHANGE USER: (f0 42 30 73 20) 4e 00 NN f7
		else if ( sysex->at(5)==0x4e && sysex->at(6)==0x10 ) {
			int program = (int)sysex->at(7);

#ifdef DEBUG
			std::cout << "program change def: " << program << std::endl;
#endif
			// Request to be sent the program data
			m_currentPreset.number = program;
			m_currentPreset.slot = PandoraPreset::PROGRAM_FACTORY;
			requestCurrentProgramData();

		}
		else if ( sysex->at(5)==0x4e && sysex->at(6)==0x00 ) {
			int program = (int)sysex->at(7);

#ifdef DEBUG
			std::cout << "program change user: " << program << std::endl;
#endif

			m_currentPreset.number = program;
			m_currentPreset.slot = PandoraPreset::PROGRAM_USER;
			requestCurrentProgramData();

		}

		// Enabled or disabled the dynamics module
		// Send from the Pandora when enabling or disabling the module.
		//Dynamics ON:		(f0 42 30 73 20) 41 02 00 00 01 f7
		//Dynamics OFF:		(f0 42 30 73 20) 41 02 00 00 00 f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x02 && sysex->at(7)==0x00 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "dynamics: " << state << std::endl;
#endif
			m_currentPreset.dynamics.enable(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_DYN, m_currentPreset);
		}

		// Changed the value of the dynamics effect type
		// Sent from the pandora when editing the dynamic effect type
		//DYNA TYPE:		(f0 42 30 73 20) 41 00 08 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x08 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "dyna type: " << state << std::endl;
#endif

			m_currentPreset.dynamics.setEffect((PresetDynamics::DynamicsTypes)state);
			setChanged();
			notifyObservers(PX5D_UPDATE_DYN, m_currentPreset);
		}

		// Changed the value of the dynamics effect parameter
		// Sent from the pandora when editing the dynamic effect parameter
		//DYNA STEP:		(f0 42 30 73 20) 41 00 0e 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x0e ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "dyna param: " << state << std::endl;
#endif
			m_currentPreset.dynamics.setParameter(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_DYN, m_currentPreset);
		}

		// Enable or disable the amp module
		// Send from the Pandora when enabling or disabling the module.
		//Amp ON:			(f0 42 30 73 20) 41 02 01 00 01 f7
		//Amp OFF:			(f0 42 30 73 20) 41 02 01 00 00 f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x02 && sysex->at(7)==0x01 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "amp: " << state << std::endl;
#endif
			m_currentPreset.amp.enable(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_AMP, m_currentPreset);
		}


		// Enable or disable the cab module
		// Send from the Pandora when enabling or disabling the module.
		//Cabinet ON:		(f0 42 30 73 20) 41 02 02 00 01 f7
		//Cabinet OFF:		(f0 42 30 73 20) 41 02 02 00 00 f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x02 && sysex->at(7)==0x02 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "cabinet: " << state << std::endl;
#endif
			m_currentPreset.cabinet.enable(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_CAB, m_currentPreset);
		}

		// Enable or disable the modulation module
		// Send from the Pandora when enabling or disabling the module.
		//Modulation ON:	(f0 42 30 73 20) 41 02 03 00 01 f7
		//Modulation OFF:	(f0 42 30 73 20) 41 02 03 00 00 f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x02 && sysex->at(7)==0x03 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "modulation: " << state << std::endl;
#endif
			m_currentPreset.modulation.enable(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_MOD, m_currentPreset);

		}

		// Enable or disable the delay module
		// Send from the Pandora when enabling or disabling the module.
		//Delay ON:			(f0 42 30 73 20) 41 02 04 00 01 f7
		//Delay OFF:		(f0 42 30 73 20) 41 02 04 00 00 f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x02 && sysex->at(7)==0x04 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "delay: " << state << std::endl;
#endif
			m_currentPreset.delay.enable(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_DELAY, m_currentPreset);

		}

		// Enable or disable the reverb module
		// Send from the Pandora when enabling or disabling the module.
		//Reverb ON:		(f0 42 30 73 20) 41 02 05 00 01 f7
		//Reverb OFF:		(f0 42 30 73 20) 41 02 05 00 00 f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x02 && sysex->at(7)==0x05 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "reverb: " << state << std::endl;
#endif
			m_currentPreset.reverb.enable(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_REVERB, m_currentPreset);
		}

		// Amp TYPE:		(f0 42 30 73 20) 41 00 09 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x09 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "amp type: " << state << std::endl;
#endif
			m_currentPreset.amp.setModel((PresetAmp::AmpTypes)state);
			setChanged();
			notifyObservers(PX5D_UPDATE_AMP, m_currentPreset);
		}

		// Amp gain:		(f0 42 30 73 20) 41 01 00 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x01 && sysex->at(7)==0x00 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "amp gain: " << state << std::endl;
#endif
			m_currentPreset.amp.setGain(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_AMP, m_currentPreset);
		}

		// BASS STEP:		(f0 42 30 73 20) 41 01 01 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x01 && sysex->at(7)==0x01 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "amp bass: " << state << std::endl;
#endif
			m_currentPreset.amp.setBass(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_AMP, m_currentPreset);
		}

		// TREBLE STEP:		(f0 42 30 73 20) 41 01 02 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x01 && sysex->at(7)==0x02 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "amp treble: " << state << std::endl;
#endif
			m_currentPreset.amp.setTreble(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_AMP, m_currentPreset);
		}

		// VOLUME STEP:		(f0 42 30 73 20) 41 01 03 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x01 && sysex->at(7)==0x03 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "amp volume: " << state << std::endl;
#endif
			m_currentPreset.amp.setVolume(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_AMP, m_currentPreset);
		}

		// MIDDLE STEP:		(f0 42 30 73 20) 41 00 0f 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x0f ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "amp middle: " << state << std::endl;
#endif
			m_currentPreset.amp.setMiddle(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_AMP, m_currentPreset);
		}

		// CAB TYPE:	(f0 42 30 73 20) 41 00 0a 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x0a ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "cab type: " << state << std::endl;
#endif
			m_currentPreset.cabinet.setCabinet((PresetCabinet::CabTypes)state);
			setChanged();
			notifyObservers(PX5D_UPDATE_CAB, m_currentPreset);
		}

		// CAB PRESENCE:	(f0 42 30 73 20 41) 00 10 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x10 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "cab presence: " << state << std::endl;
#endif
			m_currentPreset.cabinet.setParameter(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_CAB, m_currentPreset);
		}

		// MOD TYPE: (f0 42 30 73 20) 41 00 0b 00 NN  f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x0b ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "mod type: " << state << std::endl;
#endif
			m_currentPreset.modulation.setModulation((PresetModulation::ModulationTypes)state);
			setChanged();
			notifyObservers(PX5D_UPDATE_MOD, m_currentPreset);
		}

		// MOD PARAM: (f0 42 30 73 20) 41 00 11 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x11 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "mod param: " << state << std::endl;
#endif
			m_currentPreset.modulation.setParameter(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_MOD, m_currentPreset);
		}

		// DELAY TYPE:		(f0 42 30 73 20) 41 00 0c 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x0c ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "delay type: " << state << std::endl;
#endif
			m_currentPreset.delay.setDelay((PresetDelay::DelayTypes)state);
			setChanged();
			notifyObservers(PX5D_UPDATE_DELAY, m_currentPreset);
		}

		// DELAY FX LEVEL:	(f0 42 30 73 20) 41 00 12 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x12 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "delay level: " << state << std::endl;
#endif
			m_currentPreset.delay.setParameter1(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_DELAY, m_currentPreset);
		}

		// DELAY TIME:		(f0 42 30 73 20) 41 00 13 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x13 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "delay time: " << state << std::endl;
#endif
			m_currentPreset.delay.setParameter2(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_DELAY, m_currentPreset);
		}

		// REVERB TYPE:		(f0 42 30 73 20) 41 00 0d 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x0d ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "reverb type: " << state << std::endl;
#endif
			m_currentPreset.reverb.setReverb((PresetReverb::ReverbTypes)state);
			setChanged();
			notifyObservers(PX5D_UPDATE_REVERB, m_currentPreset);
		}

		// REVERB FX LEVEL:	(f0 42 30 73 20) 41 00 14 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x14 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "Reverb level: " << state << std::endl;
#endif
			m_currentPreset.reverb.setParameter(state);

			setChanged();
			notifyObservers(PX5D_UPDATE_REVERB, m_currentPreset);
		}

		 // Noise Reduction: (f0 42 30 73 20) 41 00 15 00 NN f7
		else if ( sysex->at(5)==0x41 && sysex->at(6)==0x00 && sysex->at(7)==0x15 ) {
			int state = (int)sysex->at(9);

#ifdef DEBUG
			std::cout << "Noise reduction level: " << state << std::endl;
#endif
			m_currentPreset.noiseReduction.setParameter(state);
			setChanged();
			notifyObservers(PX5D_UPDATE_NOISE, m_currentPreset);
		}


	} else {
#ifdef DEBUG
		std::cout << "unrecognized message!" << std::endl;
#endif
	}


}

/*
 * Ask Pandora to confirm identity
 * (might be used when we will allow to connect manually to a device)
 **/
void Px5dController::requestIdentity()
{
	/*
	Handshake:
	// send: F0 7E 7F 06 01 F7  (sysex identity request)
	// and we should receive: F0 7E 00 06 02 42 73 00 20 00 00 00 01 00 F7
	*/

	std::vector<unsigned char> message;
	message.push_back( 0xf0 );
	message.push_back( 0x7e );
	message.push_back( 0x7f );
	message.push_back( 0x06 );
	message.push_back( 0x01 );
	message.push_back( 0xf7 );
	sendSysex( &message, "identity request" );

}

/*
 * Ask Pandora to tell the current program
 **/
void Px5dController::requestCurrentProgram()
{
	// send: f0 42 30 73 20 12 f7
	// Reply (11): f0 42 30 73 20 42 50 XX 0f 00 f7 (default P)
	// Reply (11): f0 42 30 73 20 42 40 XX 0f 00 f7 (user U)

	std::vector<unsigned char> message;
	fillPandoraSysexHeader(&message);
	message.push_back( 0x12 );
	message.push_back( 0xf7 );
	sendSysex( &message, "request current program number" );

}

/*
 * Ask a full program dump
 **/
void Px5dController::requestFullDump()
{
	// ???? même code que current program number
	//char sysex[7] = { 0xf0, 0x42, 0x30, 0x73, 0x20, 0x12, 0xf7 };
	/*
	std::vector<unsigned char> message;
	fillPandoraSysexHeader(&message);
	message.push_back( 0x12 );
	message.push_back( 0xf7 );
	sendSysex( &message );
	*/

}

/*
 * Ask current program data
 */
void Px5dController::requestCurrentProgramData()
{
	/* send: f0 42 30 73 20 10 f7 "get current program"
	Receive (37): f0 42 30 73 20 40 0 21 33 23 25 2e 24 0 0 4e 0 0f 9 37 9 7 0 0e 0d 15 1e 10 20 0d 0 19 15 17 0e 12 f7
	Decomposed:
	   [0-6] f0 42 30 73 20 40 00 (sysex signature "sending current program")
	   [7-N] 2d 25 34 21 2c 29 23 00 4f 01 0b 08 00 0a 02 00 15 01 15 0a 0f 03 09 00 1e 18 11 13 0c f7
	   [7-N] m  e  t  a  l  i  c  00
									 4f 01 0b 08 00 0a 02 00 15 01 15 0a 0f 03 09 00
																					 1e 18 11 13 0c
																									f7
	which is identical to what is stored inside the preset files, except the ending 00 which are note stored in files.
	*/

	std::vector<unsigned char> message;
	fillPandoraSysexHeader(&message);
	message.push_back( 0x10 );
	message.push_back( 0xf7 );
	sendSysex( &message, "request current program data" );

}

/*
 * Ask to set a module state
*/
void Px5dController::setModuleState(PandoraNotification p, unsigned int value) {
	/*
	Dynamics ON:		f0 42 30 73 20 41 02 00 00 01 f7
	Dynamics OFF:		f0 42 30 73 20 41 02 00 00 00 f7

	Amp ON:			    f0 42 30 73 20 41 02 01 00 01 f7
	Amp OFF:		    f0 42 30 73 20 41 02 01 00 00 f7

	Cabinet ON:		    f0 42 30 73 20 41 02 02 00 01 f7
	Cabinet OFF:		f0 42 30 73 20 41 02 02 00 00 f7

	Modulation ON:		f0 42 30 73 20 41 02 03 00 01 f7
	Modulation OFF:		f0 42 30 73 20 41 02 03 00 00 f7

	Delay ON:		    f0 42 30 73 20 41 02 04 00 01 f7
	Delay OFF:		    f0 42 30 73 20 41 02 04 00 00 f7

	Reverb ON:		    f0 42 30 73 20 41 02 05 00 01 f7
	Reverb OFF:		    f0 42 30 73 20 41 02 05 00 00 f7
						*/
	bool enabled = value>0;

	if ( ( p==PX5D_MODULE_DYN && m_currentPreset.dynamics.getEnabled()==enabled ) ||
		 ( p==PX5D_MODULE_AMP && m_currentPreset.amp.getEnabled()==enabled ) ||
		 ( p==PX5D_MODULE_CAB && m_currentPreset.cabinet.getEnabled()==enabled ) ||
		 ( p==PX5D_MODULE_MOD && m_currentPreset.modulation.getEnabled()==enabled ) ||
		 ( p==PX5D_MODULE_DELAY && m_currentPreset.delay.getEnabled()==enabled ) ||
		 ( p==PX5D_MODULE_REVERB && m_currentPreset.reverb.getEnabled()==enabled )
		 ) {
#ifdef DEBUG
		std::cout << " mod already enabled/disabled" << std::endl;
#endif
		return;
	}

	std::vector<unsigned char> message;
	fillPandoraSysexHeader(&message);

	message.push_back( 0x41 );
	message.push_back( 0x02 );
	switch ( p ) {

	case PX5D_MODULE_DYN:
		message.push_back( 0x00 );
		m_currentPreset.dynamics.enable(enabled);
		break;

	case PX5D_MODULE_AMP:
		message.push_back( 0x01 );
		m_currentPreset.amp.enable(enabled);
		break;

	case PX5D_MODULE_CAB:
		message.push_back( 0x02 );
		m_currentPreset.cabinet.enable(enabled);
		break;

	case PX5D_MODULE_MOD:
		message.push_back( 0x03 );
		m_currentPreset.modulation.enable(enabled);
		break;

	case PX5D_MODULE_DELAY:
		message.push_back( 0x04 );
		m_currentPreset.delay.enable(enabled);
		break;

	case PX5D_MODULE_REVERB:
		message.push_back( 0x05 );
		m_currentPreset.reverb.enable(enabled);
		break;

	default:
		return;

		//TODO: handle noise as module ? value=0 => disabled
	}

	message.push_back( 0x00 );
	message.push_back( enabled?0x01:0x00 );
	message.push_back( 0xf7 );

	sendSysex( &message, "set module state: " +  p );
}


/*
 * Ask for a parameter change
 */
void Px5dController::setParamChanged(PandoraNotification p, unsigned int v) {

	if ( ( p==PX5D_MODULE_DYN_TYPE && m_currentPreset.dynamics.getEffect()==v ) ||
		 ( p==PX5D_MODULE_DYN_PARAM && m_currentPreset.dynamics.getParameter()==v ) ||

		 ( p==PX5D_MODULE_AMP_TYPE && m_currentPreset.amp.getModel()==v ) ||
		 ( p==PX5D_MODULE_AMP_PARAM1 && m_currentPreset.amp.getGain()==v ) ||
		 ( p==PX5D_MODULE_AMP_PARAM2 && m_currentPreset.amp.getBass()==v ) ||
		 ( p==PX5D_MODULE_AMP_PARAM3 && m_currentPreset.amp.getTreble()==v ) ||
		 ( p==PX5D_MODULE_AMP_PARAM4 && m_currentPreset.amp.getVolume()==v ) ||
		 ( p==PX5D_MODULE_AMP_PARAM5 && m_currentPreset.amp.getMiddle()==v ) ||

		 ( p==PX5D_MODULE_CAB_TYPE && m_currentPreset.cabinet.getCabinet()==v ) ||
		 ( p==PX5D_MODULE_CAB_PARAM && m_currentPreset.cabinet.getParameter()==v ) ||

		 ( p==PX5D_MODULE_MOD_TYPE && m_currentPreset.modulation.getModulation()==v ) ||
		 ( p==PX5D_MODULE_MOD_PARAM && m_currentPreset.modulation.getParameter()==v ) ||

		 ( p==PX5D_MODULE_DELAY_TYPE && m_currentPreset.delay.getDelay()==v ) ||
		 ( p==PX5D_MODULE_DELAY_PARAM1 && m_currentPreset.delay.getParameter1()==v ) ||
		 ( p==PX5D_MODULE_DELAY_PARAM2 && m_currentPreset.delay.getParameter2()==v ) ||

		 ( p==PX5D_MODULE_REVERB_TYPE && m_currentPreset.reverb.getReverb()==v ) ||
		 ( p==PX5D_MODULE_REVERB_PARAM && m_currentPreset.reverb.getParameter()==v ) ||

		 ( p==PX5D_MODULE_NOISE_REDUCTION && m_currentPreset.noiseReduction.getParameter()==v )

		 ) {
#ifdef DEBUG
		std::cout << " current param is already this value " << std::endl;
#endif
		return;
	}

	std::vector<unsigned char> message;
	fillPandoraSysexHeader(&message);

	switch ( p ) {

	// DYN TYPE:		(f0 42 30 73 20) 41 00 08 00 NN f7
	case PX5D_MODULE_DYN_TYPE:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x08 );
		message.push_back( 0x00 );
		m_currentPreset.dynamics.setEffect((PresetDynamics::DynamicsTypes)v);
		break;

	// DYNA STEP:		(f0 42 30 73 20) 41 00 0e 00 NN f7
	case PX5D_MODULE_DYN_PARAM:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x0e );
		message.push_back( 0x00 );
		m_currentPreset.dynamics.setParameter(v);
		break;

	// AMP TYPE: (f0 42 30 73 20) 41 00 09 00 NN f7
	case PX5D_MODULE_AMP_TYPE:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x09 );
		message.push_back( 0x00 );
		m_currentPreset.amp.setModel((PresetAmp::AmpTypes)v);
		break;

	// GAIN STEP:		(f0 42 30 73 20) 41 01 00 00 NN f7
	case PX5D_MODULE_AMP_PARAM1:
		message.push_back( 0x41 );
		message.push_back( 0x01 );
		message.push_back( 0x00 );
		message.push_back( 0x00 );
		m_currentPreset.amp.setGain(v);
		break;

	// BASS STEP:		(f0 42 30 73 20) 41 01 01 00 NN f7
	case PX5D_MODULE_AMP_PARAM2:
		message.push_back( 0x41 );
		message.push_back( 0x01 );
		message.push_back( 0x01 );
		message.push_back( 0x00 );
		m_currentPreset.amp.setBass(v);
		break;

	// TREBLE STEP:		(f0 42 30 73 20) 41 01 02 00 NN f7
	case PX5D_MODULE_AMP_PARAM3:
		message.push_back( 0x41 );
		message.push_back( 0x01 );
		message.push_back( 0x02 );
		message.push_back( 0x00 );
		m_currentPreset.amp.setTreble(v);
		break;

	// VOLUME STEP:		(f0 42 30 73 20) 41 01 03 00 NN f7
	case PX5D_MODULE_AMP_PARAM4:
		message.push_back( 0x41 );
		message.push_back( 0x01 );
		message.push_back( 0x03 );
		message.push_back( 0x00 );
		m_currentPreset.amp.setVolume(v);
		break;

	// MIDDLE STEP:		(f0 42 30 73 20) 41 00 0f 00 NN f7
	case PX5D_MODULE_AMP_PARAM5:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x0f );
		message.push_back( 0x00 );
		m_currentPreset.amp.setMiddle(v);
		break;

	// CAB TYPE:			(f0 42 30 73 20) 41 00 0a 00 NN f7
	case PX5D_MODULE_CAB_TYPE:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x0a );
		message.push_back( 0x00 );
		m_currentPreset.cabinet.setCabinet((PresetCabinet::CabTypes)v);
		break;

	// CAB PRESENCE STEP:	(f0 42 30 73 20) 41 00 10 00 NN f7
	case PX5D_MODULE_CAB_PARAM:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x10 );
		message.push_back( 0x00 );
		m_currentPreset.cabinet.setParameter(v);
		break;

	// MOD TYPE: (f0 42 30 73 20) 41 00 0b 00 01 f7
	case PX5D_MODULE_MOD_TYPE:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x0b );
		message.push_back( 0x00 );
		m_currentPreset.modulation.setModulation((PresetModulation::ModulationTypes)v);
		break;

	// MOD PARAM: (f0 42 30 73 20) 41 00 11 00 NN f7
	case PX5D_MODULE_MOD_PARAM:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x11 );
		message.push_back( 0x00 );
		m_currentPreset.modulation.setParameter(v);
		break;

	// DELAY TYPE:		(f0 42 30 73 20) 41 00 0c 00 NN f7
	case PX5D_MODULE_DELAY_TYPE:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x0c );
		message.push_back( 0x00 );
		m_currentPreset.delay.setDelay((PresetDelay::DelayTypes)v);
		break;

	// DELAY FX LEVEL:	(f0 42 30 73 20) 41 00 12 00 NN f7
	case PX5D_MODULE_DELAY_PARAM1:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x12 );
		message.push_back( 0x00 );
		m_currentPreset.delay.setParameter1(v);
		break;

	// DELAY TIME:		(f0 42 30 73 20) 41 00 13 00 NN f7
	case PX5D_MODULE_DELAY_PARAM2:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x13 );
		message.push_back( 0x00 );
		m_currentPreset.delay.setParameter2(v);
		break;

	// REVERB TYPE:		(f0 42 30 73 20) 41 00 0d 00 NN f7
	case PX5D_MODULE_REVERB_TYPE:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x0d );
		message.push_back( 0x00 );
		m_currentPreset.reverb.setReverb((PresetReverb::ReverbTypes)v);
		break;

	// REVERB FX LEVEL:	(f0 42 30 73 20) 41 00 14 00 NN f7
	case PX5D_MODULE_REVERB_PARAM:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x14 );
		message.push_back( 0x00 );
		m_currentPreset.reverb.setParameter(v);
		break;

	// Noise Reduction: (f0 42 30 73 20) 41 00 15 00 NN f7
	case PX5D_MODULE_NOISE_REDUCTION:
		message.push_back( 0x41 );
		message.push_back( 0x00 );
		message.push_back( 0x15 );
		message.push_back( 0x00 );
		m_currentPreset.noiseReduction.setParameter(v);
		break;

	default:
		return;
		//DONE!
	}

	message.push_back( v ); //TODO: validate value ?
	message.push_back( 0xf7 );

	sendSysex( &message, "parameter change" );

}


void Px5dController::SaveCurrentProgramToSlot(unsigned int slot) {

	// WRITE PROGRAM N:	(f0 42 30 73 20) 11 00 NN f7
	std::vector<unsigned char> message;
	fillPandoraSysexHeader(&message);
	message.push_back( 0x11 );
	message.push_back( 0x00 );
	message.push_back( slot );
	message.push_back( 0xf7 );
	sendSysex( &message, "save program" );
}

/*
 * Send a sysex message to Pandora
*/
void Px5dController::sendSysex(std::vector< unsigned char > *message, std::string debug ) {

	if ( !m_isConnected ) return;

#ifdef DEBUG
	std::cout << "MIDI OUT: " ;
	std::cout << setfill ('0') << setw(2);
	for ( unsigned int i=0; i<message->size(); i++ )
		std::cout << setfill ('0') << setw(2) << hex << (int)message->at(i) << " ";

	std::cout << " (" << debug << ")" << std::endl;
#endif

	m_midiOut->sendMessage(message);
}


/*
 * Sent from editor:
 5703,4588.6438,Midi Through:0,-,System exclusive,6,,f0 7e 7f 06 01 f7 (identify)
5727,4588.6896,Midi Through:0,-,System exclusive,7,,f0 42 30 73 20 12 f7 (current program number)
5734,4588.7061,Midi Through:0,-,System exclusive,7,,f0 42 30 73 20 10 f7 (current program data)
5743,4588.7230,Midi Through:0,-,System exclusive,7,,f0 42 30 73 20 36 f7 (? 66 bytes dump ?)
5771,4588.7834,Midi Through:0,-,System exclusive,7,,f0 42 30 73 20 0f f7 (?)
 *
 * 7,,f0 42 30 73 20 23 f7 (?)
 *
 */



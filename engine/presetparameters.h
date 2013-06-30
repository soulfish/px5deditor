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
#ifndef __PRESET_PARAMETERS__
#define __PRESET_PARAMETERS__

/* These notifications are used to identify commands
 * sent to the UI, and as a general purpose identifier
 * of effects, modules, parameters...
 **/
enum PandoraNotification {

	PX5D_PROGRAM_CHANGE = 1,
	PX5D_CURRENT_PROGRAM_FACTORY,
	PX5D_CURRENT_PROGRAM_USER,
	PX5D_CURRENT_PROGRAM_NAME,

	PX5D_ENABLE_DYN,
	PX5D_ENABLE_AMP,
	PX5D_ENABLE_CAB,
	PX5D_ENABLE_MOD,
	PX5D_ENABLE_DELAY,
	PX5D_ENABLE_REVERB,

	PX5D_MODULE_DYN,
	PX5D_MODULE_AMP,
	PX5D_MODULE_CAB,
	PX5D_MODULE_MOD,
	PX5D_MODULE_DELAY,
	PX5D_MODULE_REVERB,
	PX5D_MODULE_NOISE,

	PX5D_MODULE_NAME,

	PX5D_MODULE_DYN_TYPE,
	PX5D_MODULE_DYN_PARAM,

	PX5D_MODULE_AMP_TYPE,
	PX5D_MODULE_AMP_PARAM1,
	PX5D_MODULE_AMP_PARAM2,
	PX5D_MODULE_AMP_PARAM3,
	PX5D_MODULE_AMP_PARAM4,
	PX5D_MODULE_AMP_PARAM5,

	PX5D_MODULE_CAB_TYPE,
	PX5D_MODULE_CAB_PARAM,

	PX5D_MODULE_MOD_TYPE,
	PX5D_MODULE_MOD_PARAM,

	PX5D_MODULE_DELAY_TYPE,
	PX5D_MODULE_DELAY_PARAM1,
	PX5D_MODULE_DELAY_PARAM2,

	PX5D_MODULE_REVERB_TYPE,
	PX5D_MODULE_REVERB_PARAM,

	PX5D_MODULE_NOISE_REDUCTION,

	PX5D_MIDI_DUMP_COMPLETE,

	PX5D_MIDI_PROCESS_COMPLETE

	//complet
};

// Update/Notify gui
// bit flag to know what part of a preset were updated
enum PandoraUpdatedSignal {
	//TODO: voir si necessaire d'avoir un event par paramètre
	PX5D_UPDATE_NONE   = 0x0,
	PX5D_UPDATE_DYN    = 0x1,
	PX5D_UPDATE_AMP    = 0x2,
	PX5D_UPDATE_CAB    = 0x4,
	PX5D_UPDATE_MOD    = 0x8,
	PX5D_UPDATE_DELAY  = 0x10,
	PX5D_UPDATE_REVERB = 0x20,
	PX5D_UPDATE_NOISE  = 0x40,

	PX5D_UPDATE_NAME   = 0x80,

	PX5D_HANDSHAKE     = 0x100,
	PX5D_ACK           = 0x200,
	PX5D_UPDATE_MIDIDUMP      = 0x400,
	PX5D_UPDATE_MIDIDUMP_COMPLETE  = 0x800,

	PX5D_UPDATE_ALL    = 0xFFFF

};



#endif

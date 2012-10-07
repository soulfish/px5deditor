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

#include <QtCore/QEvent>
#include <QDebug>
#include <string>
#include "engine/presetparameters.h"
#include "engine/pandorapreset.h"
#include "engine/presetdynamics.h"
#include "engine/presetamp.h"
#include "engine/presetcabinet.h"
#include "engine/presetmodulation.h"
#include "engine/presetdelay.h"
#include "engine/presetreverb.h"
#include "engine/presetnoisereduction.h"

#ifndef EVENTS_H
#define EVENTS_H

const QEvent::Type PX5DProgramEventType = static_cast<QEvent::Type>(QEvent::User + PX5D_PROGRAM_CHANGE);
const QEvent::Type PX5DDynamicsEventType = static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_DYN);
const QEvent::Type PX5DAmpEventType = static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_AMP);
const QEvent::Type PX5DCabEventType = static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_CAB);
const QEvent::Type PX5DModulationEventType = static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_MOD);
const QEvent::Type PX5DDelayEventType = static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_DELAY);
const QEvent::Type PX5DReverbEventType = static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_REVERB);
const QEvent::Type PX5DNoiseReductionEventType = static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_NOISE);
const QEvent::Type PX5DNameChangeEventType = static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_NAME);

/*
 * Event for propagating a program change event
 */
class PX5DProgramEvent : public QEvent {
public:

	PX5DProgramEvent(unsigned int pgNumber, const QString& pgName, PandoraPreset::ProgramBank slot) :
		QEvent(static_cast<QEvent::Type>(QEvent::User + PX5D_PROGRAM_CHANGE)), m_pgNumber(pgNumber), m_pgName(pgName), m_slot(slot) {
	}

	unsigned int getProgramNumber() const { return m_pgNumber; }
	const QString& getProgramName() const { return m_pgName; }
	PandoraPreset::ProgramBank getSlot() const { return m_slot; }

protected:
	unsigned int m_pgNumber;
	QString m_pgName;
	PandoraPreset::ProgramBank m_slot;
};


/*
 * Event for propagating a dynamics change event
 */
class PX5DDynamicsEvent : public QEvent {
public:
	PX5DDynamicsEvent(bool enabled, PresetDynamics::DynamicsTypes effect, unsigned int param,
					  const QString& paramName, unsigned int pMin, unsigned int pMax ) :
		QEvent(static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_DYN)), m_enabled(enabled), m_effect(effect), m_param(param),
		m_paramName(paramName), m_pMin(pMin), m_pMax(pMax) {}

	bool getEnabled() const { return m_enabled; }
	PresetDynamics::DynamicsTypes getEffect() const { return m_effect; }
	unsigned int getParameter() const { return m_param; }
	QString getParamName() const { return m_paramName; }
	unsigned int getMinParam() const { return m_pMin; }
	unsigned int getMaxParam() const { return m_pMax; }

protected:
	bool m_enabled;
	PresetDynamics::DynamicsTypes m_effect;
	unsigned int m_param;
	QString m_paramName;
	unsigned int m_pMin;
	unsigned int m_pMax;
};

/*
 * Event for propagating an Amp change event
*/
class PX5DAmpEvent : public QEvent {
public:
	PX5DAmpEvent(bool enabled,
				 PresetAmp::AmpTypes model,
				 unsigned int gain,
				 unsigned int bass,
				 unsigned int treble,
				 unsigned int volume,
				 unsigned int middle,
				 const QString& p1name,
				 unsigned int p1min,
				 unsigned int p1max,
				 const QString& p2name,
				 unsigned int p2min,
				 unsigned int p2max,
				 const QString& p3name,
				 unsigned int p3min,
				 unsigned int p3max,
				 const QString& p4name,
				 unsigned int p4min,
				 unsigned int p4max,
				 const QString& p5name,
				 unsigned int p5min,
				 unsigned int p5max
				  ) :
		QEvent(static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_AMP)),
		m_enabled(enabled),
		m_model(model),
		m_gain(gain),
		m_bass(bass),
		m_treble(treble),
		m_volume(volume),
		m_middle(middle),
		m_param1Name(p1name),
		m_param1Min(p1min),
		m_param1Max(p1max),
		m_param2Name(p2name),
		m_param2Min(p2min),
		m_param2Max(p2max),
		m_param3Name(p3name),
		m_param3Min(p3min),
		m_param3Max(p3max),
		m_param4Name(p4name),
		m_param4Min(p4min),
		m_param4Max(p4max),
		m_param5Name(p5name),
		m_param5Min(p5min),
		m_param5Max(p5max) {}

	bool getEnabled() const { return m_enabled; }

	PresetAmp::AmpTypes getModel() const { return m_model; }

	unsigned int getGain() const { return m_gain; }
	unsigned int getBass() const { return m_bass; }
	unsigned int getTreble() const { return m_treble; }
	unsigned int getVolume() const { return m_volume; }
	unsigned int getMiddle() const { return m_middle; }

	unsigned int getParamGainMin() const { return m_param1Min; }
	unsigned int getParamBassMin() const { return m_param2Min; }
	unsigned int getParamTrebleMin() const { return m_param3Min; }
	unsigned int getParamVolumeMin() const { return m_param4Min; }
	unsigned int getParamMiddleMin() const { return m_param5Min; }

	unsigned int getParamGainMax() const { return m_param1Max; }
	unsigned int getParamBassMax() const { return m_param2Max; }
	unsigned int getParamTrebleMax() const { return m_param3Max; }
	unsigned int getParamVolumeMax() const { return m_param4Max; }
	unsigned int getParamMiddleMax() const { return m_param5Max; }

	QString getParamGainName() const { return m_param1Name; }
	QString getParamBassName() const { return m_param2Name; }
	QString getParamTrebleName() const { return m_param3Name; }
	QString getParamVolumeName() const { return m_param4Name; }
	QString getParamMiddleName() const { return m_param5Name; }


protected:
	bool m_enabled;
	PresetAmp::AmpTypes m_model;
	unsigned int m_gain;
	unsigned int m_bass;
	unsigned int m_treble;
	unsigned int m_volume;
	unsigned int m_middle;
	QString m_param1Name;
	unsigned int m_param1Min;
	unsigned int m_param1Max;
	QString m_param2Name;
	unsigned int m_param2Min;
	unsigned int m_param2Max;
	QString m_param3Name;
	unsigned int m_param3Min;
	unsigned int m_param3Max;
	QString m_param4Name;
	unsigned int m_param4Min;
	unsigned int m_param4Max;
	QString m_param5Name;
	unsigned int m_param5Min;
	unsigned int m_param5Max;

};

/*
 * Event for propagating a Cab change event
*/
class PX5DCabEvent : public QEvent {
public:
	PX5DCabEvent(bool enabled, PresetCabinet::CabTypes cabinet, unsigned int param,
				 const QString& paramName, unsigned int pMin, unsigned int pMax ) :
		QEvent(static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_CAB)), m_enabled(enabled), m_cabinet(cabinet), m_param(param),
		m_paramName(paramName), m_pMin(pMin), m_pMax(pMax) {}

	bool getEnabled() const { return m_enabled; }
	PresetCabinet::CabTypes getCabinet() const { return m_cabinet; }
	unsigned int getParameter() const { return m_param; }
	QString getParamName() const { return m_paramName; }
	unsigned int getMinParam() const { return m_pMin; }
	unsigned int getMaxParam() const { return m_pMax; }

protected:
	bool m_enabled;
	PresetCabinet::CabTypes m_cabinet;
	unsigned int m_param;
	QString m_paramName;
	unsigned int m_pMin;
	unsigned int m_pMax;
};

/*
 * Event for propagating a Modulation change event
*/
class PX5DModulationEvent : public QEvent {
public:
	PX5DModulationEvent(bool enabled, PresetModulation::ModulationTypes modulation, unsigned int param,
				 const QString& paramName, unsigned int pMin, unsigned int pMax ) :
		QEvent(static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_MOD)), m_enabled(enabled), m_modulation(modulation), m_param(param),
		m_paramName(paramName), m_pMin(pMin), m_pMax(pMax) {}

	bool getEnabled() const { return m_enabled; }
	PresetModulation::ModulationTypes getModulation() const { return m_modulation; }
	unsigned int getParameter() const { return m_param; }
	QString getParamName() const { return m_paramName; }
	unsigned int getMinParam() const { return m_pMin; }
	unsigned int getMaxParam() const { return m_pMax; }

protected:
	bool m_enabled;
	PresetModulation::ModulationTypes m_modulation;
	unsigned int m_param;
	QString m_paramName;
	unsigned int m_pMin;
	unsigned int m_pMax;
};

/*
 * Event for propagating a Delay change event
*/

class PX5DDelayEvent : public QEvent {
public:
	PX5DDelayEvent(bool enabled, PresetDelay::DelayTypes delay,
				 unsigned int param1, const QString& param1Name, unsigned int p1Min, unsigned int p1Max,
				 unsigned int param2, const QString& param2Name, unsigned int p2Min, unsigned int p2Max ) :
		QEvent(static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_DELAY)), m_enabled(enabled), m_delay(delay),
		m_param1(param1), m_param1Name(param1Name), m_p1Min(p1Min), m_p1Max(p1Max),
		m_param2(param2), m_param2Name(param2Name), m_p2Min(p2Min), m_p2Max(p2Max) {}

	bool getEnabled() const { return m_enabled; }
	PresetDelay::DelayTypes getDelay() const { return m_delay; }
	unsigned int getParameter1() const { return m_param1; }
	QString getParam1Name() const { return m_param1Name; }
	unsigned int getMinParam1() const { return m_p1Min; }
	unsigned int getMaxParam1() const { return m_p1Max; }

	unsigned int getParameter2() const { return m_param2; }
	QString getParam2Name() const { return m_param2Name; }
	unsigned int getMinParam2() const { return m_p2Min; }
	unsigned int getMaxParam2() const { return m_p2Max; }

protected:
	bool m_enabled;
	PresetDelay::DelayTypes m_delay;
	unsigned int m_param1;
	QString m_param1Name;
	unsigned int m_p1Min;
	unsigned int m_p1Max;
	unsigned int m_param2;
	QString m_param2Name;
	unsigned int m_p2Min;
	unsigned int m_p2Max;
};


/*
 * Event for propagating a reverb change event
 */
class PX5DReverbEvent : public QEvent {
public:
	PX5DReverbEvent(bool enabled, PresetReverb::ReverbTypes reverb, unsigned int param,
					  const QString& paramName, unsigned int pMin, unsigned int pMax ) :
		QEvent(static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_REVERB)), m_enabled(enabled), m_reverb(reverb), m_param(param),
		m_paramName(paramName), m_pMin(pMin), m_pMax(pMax) {}

	bool getEnabled() const { return m_enabled; }
	PresetReverb::ReverbTypes getReverb() const { return m_reverb; }
	unsigned int getParameter() const { return m_param; }
	QString getParamName() const { return m_paramName; }
	unsigned int getMinParam() const { return m_pMin; }
	unsigned int getMaxParam() const { return m_pMax; }

protected:
	bool m_enabled;
	PresetReverb::ReverbTypes m_reverb;
	unsigned int m_param;
	QString m_paramName;
	unsigned int m_pMin;
	unsigned int m_pMax;
};


/*
 * Event for propagating a noise reduction change event
 */
class PX5DNoiseReductionEvent : public QEvent {
public:
	PX5DNoiseReductionEvent(bool enabled, unsigned int param,
					  const QString& paramName, unsigned int pMin, unsigned int pMax ) :
		QEvent(static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_NOISE)), m_enabled(enabled),  m_param(param),
		m_paramName(paramName), m_pMin(pMin), m_pMax(pMax) {}

	bool getEnabled() const { return m_enabled; }
	unsigned int getParameter() const { return m_param; }
	QString getParamName() const { return m_paramName; }
	unsigned int getMinParam() const { return m_pMin; }
	unsigned int getMaxParam() const { return m_pMax; }

protected:
	bool m_enabled;
	unsigned int m_param;
	QString m_paramName;
	unsigned int m_pMin;
	unsigned int m_pMax;
};

/*
 * Event for propagating a name change event
 */
class PX5DNameChangeEvent : public QEvent {
public:
	PX5DNameChangeEvent(const QString& newName) :
		QEvent(static_cast<QEvent::Type>(QEvent::User + PX5D_MODULE_NAME)), m_name(newName) {}

	QString getName() const { return m_name; }

protected:
	QString m_name;
};


#endif // EVENTS_H

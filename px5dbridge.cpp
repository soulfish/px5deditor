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

#include <QEvent>
#include <QApplication>
#include <QDebug>

#include "px5dbridge.h"
#include "events.h"

PX5DBridge::PX5DBridge() {

	m_px5dController = new Px5dController();
	m_px5dController->addObserver(*this);
}

PX5DBridge::~PX5DBridge() {
	delete m_px5dController;
}

bool PX5DBridge::initialize(QObject *parent) {
	m_parent = parent;
	return m_px5dController->connectToPandora();
}

Px5dController *PX5DBridge::controller() {
	return m_px5dController;
}

void PX5DBridge::update(PandoraObservable* o, PandoraUpdatedSignal bitflag, PandoraPreset &p) {

	QEvent* ev = 0;

	if ( bitflag==PX5D_HANDSHAKE ) {
		//TODO
	}

	if ( bitflag==PX5D_ACK ) {
		//TODO
	}

	if ( bitflag==PX5D_UPDATE_ALL ) {
		qDebug() << " got program update call ";

		ev = new PX5DProgramEvent( p.number, QString(p.name.c_str()), p.slot );
		QApplication::postEvent(m_parent, ev);
	}


	if ( bitflag&PX5D_UPDATE_DYN || bitflag==PX5D_UPDATE_ALL ) {

		qDebug() << " got dyn update call ";

		ev = new PX5DDynamicsEvent(
				 p.dynamics.getEnabled(),
				 p.dynamics.getEffect(),
				 p.dynamics.getParameter(),
				 QString(p.dynamics.getParamName()),
				 p.dynamics.getMinParam(),
				 p.dynamics.getMaxParam() );

		QApplication::postEvent(m_parent, ev);
	}


	if ( bitflag&PX5D_UPDATE_AMP|| bitflag==PX5D_UPDATE_ALL ) {

		qDebug() << " got amp update call ";

		ev = new PX5DAmpEvent(
				 p.amp.getEnabled(),
				 p.amp.getModel(),
				 p.amp.getGain(),
				 p.amp.getBass(),
				 p.amp.getTreble(),
				 p.amp.getVolume(),
				 p.amp.getMiddle(),
				 p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_GAIN),
				 p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_GAIN),
				 p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_GAIN),
				 p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_BASS),
				 p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_BASS),
				 p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_BASS),
				 p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_TREBLE),
				 p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_TREBLE),
				 p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_TREBLE),
				 p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_VOLUME),
				 p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_VOLUME),
				 p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_VOLUME),
				 p.amp.getParamName(p.amp.getModel(), PresetAmp::AMP_MIDDLE),
				 p.amp.getMinParam(p.amp.getModel(), PresetAmp::AMP_MIDDLE),
				 p.amp.getMaxParam(p.amp.getModel(), PresetAmp::AMP_MIDDLE)
		 );

		QApplication::postEvent(m_parent, ev);
	}

	if ( bitflag&PX5D_UPDATE_CAB|| bitflag==PX5D_UPDATE_ALL ) {

		qDebug() << " got cab update call ";

		ev = new PX5DCabEvent(
				 p.cabinet.getEnabled(),
				 p.cabinet.getCabinet(),
				 p.cabinet.getParameter(),
				 p.cabinet.getParamName(),
				 p.cabinet.getMinParam(),
				 p.cabinet.getMaxParam()
				 );

		QApplication::postEvent(m_parent, ev);
	}


	if ( bitflag&PX5D_UPDATE_MOD|| bitflag==PX5D_UPDATE_ALL ) {

		qDebug() << " got mod update call ";

		ev = new PX5DModulationEvent(
				 p.modulation.getEnabled(),
				 p.modulation.getModulation(),
				 p.modulation.getParameter(),
				 p.modulation.getParamName(),
				 p.modulation.getMinParam(),
				 p.modulation.getMaxParam()
		);

		QApplication::postEvent(m_parent, ev);
	}


	if ( bitflag&PX5D_UPDATE_DELAY|| bitflag==PX5D_UPDATE_ALL ) {

		qDebug() << " got delay update call ";

		ev = new PX5DDelayEvent(
				p.delay.getEnabled(),
				p.delay.getDelay(),
				p.delay.getParameter1(),
				p.delay.getParam1Name(),
				p.delay.getMinParam1(),
				p.delay.getMaxParam1(),
				p.delay.getParameter2(),
				p.delay.getParam2Name(),
				p.delay.getMinParam2(),
				p.delay.getMaxParam2()

		);

		QApplication::postEvent(m_parent, ev);

	}


	if ( bitflag&PX5D_UPDATE_REVERB|| bitflag==PX5D_UPDATE_ALL ) {

		qDebug() << " got reverb update call ";

		ev = new PX5DReverbEvent(
				 p.reverb.getEnabled(),
				 p.reverb.getReverb(),
				 p.reverb.getParameter(),
				 p.reverb.getParamName(),
				 p.reverb.getMinParam(),
				 p.reverb.getMaxParam()
			 );

		QApplication::postEvent(m_parent, ev);
	}



	if ( bitflag&PX5D_UPDATE_NOISE|| bitflag==PX5D_UPDATE_ALL ) {

		qDebug() << " got noise reduction update call ";

		ev = new PX5DNoiseReductionEvent(
				 p.noiseReduction.getEnabled(),
				 p.noiseReduction.getParameter(),
				 p.noiseReduction.getParamName(),
				 p.noiseReduction.getMinParam(),
				 p.noiseReduction.getMaxParam()
			 );
		QApplication::postEvent(m_parent, ev);

	}

}

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


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QMessageBox>
#include <QDebug>
#include "engine/presetparameters.h"
#include "events.h"
#include "aboutdialog.h"
#include "constants.h"
#include <sys/utsname.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	m_dlgAbout = 0;

	ui->setupUi(this);

	setWindowTitle(QString("Pandora PX5D Editor for Linux - ") + PGM_VERSION);

	// Fill combo box elements
	for ( int i=PresetDynamics::COMPRESSOR; i<PresetDynamics::END; ++i ) {
		ui->dynamicsType->addItem( QString( PresetDynamics::Effects[i].fullName.c_str() ) );
	}

	for ( int i=PresetAmp::BTQCLN; i<PresetAmp::END; ++i ) {
		ui->ampType->addItem( QString( PresetAmp::Amps[i].fullName.c_str() ) );
	}

	for ( int i=PresetCabinet::TWEED1x8; i<PresetCabinet::END; ++i ) {
		ui->cabinetType->addItem( QString( PresetCabinet::Cabs[i].fullName.c_str() ) );
	}

	for ( int i=PresetModulation::CHORUS1; i<PresetModulation::END; ++i ) {
		ui->modulationType->addItem( QString( PresetModulation::Modulations[i].fullName.c_str() ) );
	}

	for ( int i=PresetDelay::SLAP1; i<PresetDelay::END; ++i ) {
		ui->delayType->addItem( QString( PresetDelay::Delays[i].fullName.c_str() ) );
	}

	for ( int i=PresetReverb::DRYAIR; i<PresetReverb::END; ++i ) {
		ui->reverbType->addItem( QString( PresetReverb::Reverbs[i].fullName.c_str() ) );
	}

	//ui->noiseReductionParamName->setText( QString( m_px5dController->Preset()->noiseReduction.getParamName()) );

	// FIXME: set wheelScrollLines(1) for all Dials !!!
	// FIXME: disable scroll wheel on program number
}

MainWindow::~MainWindow() {
	delete ui;
}


void MainWindow::on_connectToPandora_released()
{
	if ( bridge.initialize(this) ) {
		ui->px5dStatusIn->setText( QString(bridge.controller()->pandoraInPortName().c_str()) );
		ui->px5dStatusOut->setText( QString(bridge.controller()->pandoraOutPortName().c_str()) );

	} else {

		utsname kernelinfo;
		uname(&kernelinfo);

		QMessageBox msgBox;
		msgBox.setText( "Could not connect to PX5D unit!");
		msgBox.setInformativeText( QString("Please check that the device is connected to USB and powered on. ")+
								   QString("Please note that you need at least Linux kernel 3. Your version is: ") +
									QString(kernelinfo.release) );
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();

		ui->px5dStatusIn->setText( tr("Cannot connect to Pandora...") );
		ui->px5dStatusOut->setText( tr("Cannot connect to Pandora...") );
	}

}

void MainWindow::on_programNumber_valueChanged(int v) {
	bridge.controller()->setProgramNumber(v, ui->programBankFactory->isChecked()?PandoraPreset::PROGRAM_FACTORY : PandoraPreset::PROGRAM_USER);
}


void MainWindow::on_dynamicsParamDial_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_DYN_PARAM, v );
	updateParamTextValue(PX5D_MODULE_DYN_PARAM, v);
}

void MainWindow::on_dynamicsType_activated(int index) {
	bridge.controller()->setParamChanged( PX5D_MODULE_DYN_TYPE, index );
}

void MainWindow::on_programBankFactory_clicked() {
	bridge.controller()->setProgramNumber( ui->programNumber->value(), PandoraPreset::PROGRAM_FACTORY );
}

void MainWindow::on_programBankUser_clicked() {
	bridge.controller()->setProgramNumber( ui->programNumber->value(), PandoraPreset::PROGRAM_USER );
}

void MainWindow::on_ampType_activated(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_AMP_TYPE, v );
}

void MainWindow::on_ampParamDial1_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_AMP_PARAM1, v );
	updateParamTextValue(PX5D_MODULE_AMP_PARAM1, v);
}

void MainWindow::on_ampParamDial2_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_AMP_PARAM2, v );
	updateParamTextValue(PX5D_MODULE_AMP_PARAM2, v);
}

void MainWindow::on_ampParamDial3_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_AMP_PARAM3, v );
	updateParamTextValue(PX5D_MODULE_AMP_PARAM3, v);
}

void MainWindow::on_ampParamDial4_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_AMP_PARAM4, v );
	updateParamTextValue(PX5D_MODULE_AMP_PARAM4, v);
}

void MainWindow::on_ampParamDial5_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_AMP_PARAM5, v );
	updateParamTextValue(PX5D_MODULE_AMP_PARAM5, v);
}

void MainWindow::on_cabinetType_activated(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_CAB_TYPE, v );
}

void MainWindow::on_cabinetParamDial_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_CAB_PARAM, v );
	updateParamTextValue(PX5D_MODULE_CAB_PARAM, v);
}

void MainWindow::on_modulationType_activated(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_MOD_TYPE, v );
}

void MainWindow::on_modulationParamDial_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_MOD_PARAM, v );
	updateParamTextValue(PX5D_MODULE_MOD_PARAM, v);
}

void MainWindow::on_delayType_activated(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_DELAY_TYPE, v );
}

void MainWindow::on_delayParamDial_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_DELAY_PARAM1, v );
	updateParamTextValue(PX5D_MODULE_DELAY_PARAM1, v);
}

void MainWindow::on_delayParamDial2_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_DELAY_PARAM2, v );
	updateParamTextValue(PX5D_MODULE_DELAY_PARAM2, v);
}

void MainWindow::on_reverbType_activated(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_REVERB_TYPE, v );
}

void MainWindow::on_reverbParamDial_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_REVERB_PARAM, v );
	updateParamTextValue(PX5D_MODULE_REVERB_PARAM, v);
}

void MainWindow::on_noiseReductionParamDial_valueChanged(int v) {
	bridge.controller()->setParamChanged( PX5D_MODULE_NOISE_REDUCTION, v );
	updateParamTextValue(PX5D_MODULE_NOISE_REDUCTION, v);
	ui->noiseReductionBox->setChecked(v>0);
}

void MainWindow::on_writeProgram_released() {

	//TODO: implement selecting destination program via dedicated dialog

	if ( ui->programBankFactory->isChecked() ) {

		QMessageBox msgBox;
		msgBox.setText("Save Program to Pandora");
		msgBox.setInformativeText("This program is a Factory program and will be saved to the User progam of same number. Is this ok ?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();

		switch (ret) {
		case QMessageBox::Save:
			bridge.controller()->SaveCurrentProgramToSlot( ui->programNumber->value() );
			break;
		case QMessageBox::Cancel:
			break;
		default:
			break;
		}

	} else {

		QMessageBox msgBox;
		msgBox.setText("Save Program to Pandora");
		msgBox.setInformativeText("This program will overwrite the user progam of same number. Is this ok ?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();

		switch (ret) {
		case QMessageBox::Save:
			bridge.controller()->SaveCurrentProgramToSlot( ui->programNumber->value() );
			break;
		case QMessageBox::Cancel:
			break;
		default:
			break;
		}

	}
}

void MainWindow::on_actionQuit_triggered() {
	exit(0);
}

void MainWindow::on_actionAbout_triggered() {
	about()->exec();
}

AboutDialog* MainWindow::about() {
	if (m_dlgAbout == 0) {
		m_dlgAbout = new AboutDialog(this);
	}
	return m_dlgAbout;
}

void MainWindow::on_dynamicsBox_toggled(bool v) {
	bridge.controller()->setModuleState( PX5D_MODULE_DYN, v!=Qt::Unchecked );
}

void MainWindow::on_amplifierBox_toggled(bool v) {
	bridge.controller()->setModuleState( PX5D_MODULE_AMP, v!=Qt::Unchecked );
}

void MainWindow::on_cabinetBox_toggled(bool v) {
	bridge.controller()->setModuleState( PX5D_MODULE_CAB, v!=Qt::Unchecked );
}

void MainWindow::on_modulationBox_toggled(bool v) {
	bridge.controller()->setModuleState( PX5D_MODULE_MOD, v!=Qt::Unchecked );
}

void MainWindow::on_delayBox_toggled(bool v) {
	bridge.controller()->setModuleState( PX5D_MODULE_DELAY, v!=Qt::Unchecked );
}

void MainWindow::on_reverbBox_toggled(bool v) {
	bridge.controller()->setModuleState( PX5D_MODULE_REVERB, v!=Qt::Unchecked );
}

void MainWindow::updateParamTextValue(PandoraNotification p, unsigned int v) {

	const char* paramValueAsText = "";
	switch (p) {

	case PX5D_MODULE_DYN_PARAM:
		paramValueAsText = bridge.controller()->Preset()->dynamics.getParamStringValue(v);
		ui->dynamicsParam->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_AMP_PARAM1:
		paramValueAsText = bridge.controller()->Preset()->amp.getParamStringValue(PresetAmp::AMP_GAIN, v);
		ui->ampParam1->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_AMP_PARAM2:
		paramValueAsText = bridge.controller()->Preset()->amp.getParamStringValue(PresetAmp::AMP_BASS, v);
		ui->ampParam2->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_AMP_PARAM3:
		paramValueAsText = bridge.controller()->Preset()->amp.getParamStringValue(PresetAmp::AMP_TREBLE, v);
		ui->ampParam3->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_AMP_PARAM4:
		paramValueAsText = bridge.controller()->Preset()->amp.getParamStringValue(PresetAmp::AMP_VOLUME, v);
		ui->ampParam4->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_AMP_PARAM5:
		paramValueAsText = bridge.controller()->Preset()->amp.getParamStringValue(PresetAmp::AMP_MIDDLE, v);
		ui->ampParam5->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_CAB_PARAM:
		paramValueAsText = bridge.controller()->Preset()->cabinet.getParamStringValue(v);
		ui->cabinetParam->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_MOD_PARAM:
		paramValueAsText = bridge.controller()->Preset()->modulation.getParamStringValue(v);
		ui->modulationParam->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_REVERB_PARAM:
		paramValueAsText = bridge.controller()->Preset()->reverb.getParamStringValue(v);
		ui->reverbParam->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_DELAY_PARAM1:
		paramValueAsText = bridge.controller()->Preset()->delay.getParam1StringValue(v);
		ui->delayParam->setText(QString(paramValueAsText));
		break;
	case PX5D_MODULE_DELAY_PARAM2:
		paramValueAsText = bridge.controller()->Preset()->delay.getParam2StringValue(v);
		ui->delayParam2->setText(QString(paramValueAsText));
		break;

	case PX5D_MODULE_NOISE_REDUCTION:
		paramValueAsText = bridge.controller()->Preset()->noiseReduction.getParamStringValue(v);
		ui->noiseReductionParam->setText(QString(paramValueAsText));
		break;

	default:
		break;
	}
}


void MainWindow::customEvent( QEvent *event )
{
	if ( event->type() == PX5DProgramEventType ) {
		PX5DProgramEvent *ev = static_cast<PX5DProgramEvent*>(event);
		ui->programName->setText( ev->getProgramName() );
		ui->programNumber->setValue( ev->getProgramNumber() );
		if ( ev->getSlot()==PandoraPreset::PROGRAM_FACTORY ) {
			ui->programBankFactory->setChecked(true);
		}
		else if ( ev->getSlot()==PandoraPreset::PROGRAM_USER ) {
			ui->programBankUser->setChecked(true);
		}
	}

	else if ( event->type() == PX5DDynamicsEventType ) {
		PX5DDynamicsEvent *ev = static_cast<PX5DDynamicsEvent*>(event);

		ui->dynamicsBox->setChecked( ev->getEnabled() );
		if ( ev->getEffect() != ui->dynamicsType->currentIndex() &&
			 (int)ev->getParameter() == ui->dynamicsParamDial->value() ) {
			// if effect changed, but param did not, force param value update!
			updateParamTextValue(PX5D_MODULE_DYN_PARAM, ev->getParameter());
		}
		ui->dynamicsType->setCurrentIndex( ev->getEffect() );
		ui->dynamicsParamName->setText( ev->getParamName() );
		ui->dynamicsParamDial->setMinimum( ev->getMinParam() );
		ui->dynamicsParamDial->setMaximum( ev->getMaxParam() );
		ui->dynamicsParamDial->setValue( ev->getParameter() );

		//ui->dynamicsParam will auto change as we change the dial param

	}

	else if ( event->type() == PX5DAmpEventType ) {

		PX5DAmpEvent *ev = static_cast<PX5DAmpEvent*>(event);

		ui->amplifierBox->setChecked(ev->getEnabled());

		if ( ev->getModel() != ui->ampType->currentIndex() ) {
			if ( (int)ev->getGain() == ui->ampParamDial1->value() ) {
				updateParamTextValue(PX5D_MODULE_AMP_PARAM1, ev->getGain());
			}
			if ( (int)ev->getBass() == ui->ampParamDial2->value() ) {
				updateParamTextValue(PX5D_MODULE_AMP_PARAM2, ev->getBass());
			}
			if ( (int)ev->getTreble() == ui->ampParamDial3->value() ) {
				updateParamTextValue(PX5D_MODULE_AMP_PARAM3, ev->getTreble());
			}
			if ( (int)ev->getVolume() == ui->ampParamDial4->value() ) {
				updateParamTextValue(PX5D_MODULE_AMP_PARAM4, ev->getVolume());
			}
			if ( (int)ev->getMiddle() == ui->ampParamDial5->value() ) {
				updateParamTextValue(PX5D_MODULE_AMP_PARAM5, ev->getMiddle());
			}
		}

		ui->ampType->setCurrentIndex( ev->getModel() );

		//Gain, Bass, Treble, Volume, Middle
		int min	= ev->getParamGainMin();
		int max	= ev->getParamGainMax();

		ui->ampParamName1->setText( ev->getParamGainName() );
		ui->ampParamDial1->setMinimum( min );
		ui->ampParamDial1->setMaximum( max );
		ui->ampParamDial1->setValue( ev->getGain() );

		min	= ev->getParamBassMin();
		max	= ev->getParamBassMax();

		ui->ampParamName2->setText( ev->getParamBassName()  );
		ui->ampParamDial2->setMinimum( min );
		ui->ampParamDial2->setMaximum( max );
		ui->ampParamDial2->setValue( ev->getBass() );

		min	= ev->getParamTrebleMin();
		max	= ev->getParamTrebleMax();

		ui->ampParamName3->setText( ev->getParamTrebleName()  );
		ui->ampParamDial3->setMinimum( min );
		ui->ampParamDial3->setMaximum( max );
		ui->ampParamDial3->setValue( ev->getTreble() );

		min	= ev->getParamVolumeMin();
		max	= ev->getParamVolumeMax();

		ui->ampParamName4->setText( ev->getParamVolumeName() );
		ui->ampParamDial4->setMinimum( min );
		ui->ampParamDial4->setMaximum( max );
		ui->ampParamDial4->setValue( ev->getVolume() );

		min	= ev->getParamMiddleMin();
		max	= ev->getParamMiddleMax();

		ui->ampParamName5->setText( ev->getParamMiddleName()  );
		ui->ampParamDial5->setMinimum( min );
		ui->ampParamDial5->setMaximum( max );
		ui->ampParamDial5->setValue( ev->getMiddle() );
	}


	else if ( event->type() == PX5DCabEventType ) {

		PX5DCabEvent *ev = static_cast<PX5DCabEvent*>(event);

		ui->cabinetBox->setChecked(ev->getEnabled());

		if ( ev->getCabinet() != ui->cabinetType->currentIndex() &&
			 (int)ev->getParameter() == ui->cabinetParamDial->value() ) {
			updateParamTextValue(PX5D_MODULE_CAB_PARAM, ev->getParameter());
		}

		ui->cabinetType->setCurrentIndex( ev->getCabinet() );
		ui->cabinetParamName->setText( ev->getParamName() );
		ui->cabinetParamDial->setMinimum( ev->getMinParam() );
		ui->cabinetParamDial->setMaximum( ev->getMaxParam() );
		ui->cabinetParamDial->setValue( ev->getParameter() );

	}

	else if ( event->type() == PX5DModulationEventType ) {

		PX5DModulationEvent *ev = static_cast<PX5DModulationEvent*>(event);

		ui->modulationBox->setChecked(ev->getEnabled());

		if ( ev->getModulation() != ui->modulationType->currentIndex() &&
			 (int)ev->getParameter() == ui->modulationParamDial->value() ) {
			updateParamTextValue(PX5D_MODULE_MOD_PARAM, ev->getParameter());
		}

		ui->modulationType->setCurrentIndex( ev->getModulation() );
		ui->modulationParamName->setText( ev->getParamName() );
		ui->modulationParamDial->setMinimum( ev->getMinParam() );
		ui->modulationParamDial->setMaximum( ev->getMaxParam() );
		ui->modulationParamDial->setValue( ev->getParameter() );
	}

	else if ( event->type() == PX5DDelayEventType ) {

		PX5DDelayEvent *ev = static_cast<PX5DDelayEvent*>(event);

		ui->delayBox->setChecked(ev->getEnabled());

		if (  ev->getDelay() != ui->delayType->currentIndex() ) {
			if ( (int)ev->getParameter1() == ui->delayParamDial->value() ) {
				updateParamTextValue(PX5D_MODULE_DELAY_PARAM1, ev->getParameter1());
			}
			if ( (int)ev->getParameter2() == ui->delayParamDial2->value() ) {
				updateParamTextValue(PX5D_MODULE_DELAY_PARAM2, ev->getParameter2());
			}
		}

		ui->delayType->setCurrentIndex( ev->getDelay() );
		ui->delayParamName->setText( ev->getParam1Name() );
		ui->delayParamDial->setMinimum( ev->getMinParam1() );
		ui->delayParamDial->setMaximum( ev->getMaxParam1() );
		ui->delayParamDial->setValue( ev->getParameter1() );

		ui->delayParamName2->setText( ev->getParam2Name() );
		ui->delayParamDial2->setMinimum( ev->getMinParam2() );
		ui->delayParamDial2->setMaximum( ev->getMaxParam2() );
		ui->delayParamDial2->setValue( ev->getParameter2() );

	}

	else if ( event->type() == PX5DReverbEventType ) {

		PX5DReverbEvent *ev = static_cast<PX5DReverbEvent*>(event);

		ui->reverbBox->setChecked(ev->getEnabled());

		if ( ev->getReverb() != ui->reverbType->currentIndex() &&
			 (int)ev->getParameter() == ui->reverbParamDial->value() ) {
			updateParamTextValue(PX5D_MODULE_REVERB_PARAM, ev->getParameter());
		}

		ui->reverbType->setCurrentIndex( ev->getReverb() );
		ui->reverbParamName->setText( ev->getParamName() );
		ui->reverbParamDial->setMinimum( ev->getMinParam() );
		ui->reverbParamDial->setMaximum( ev->getMaxParam() );
		ui->reverbParamDial->setValue( ev->getParameter() );
	}

	else if ( event->type() == PX5DNoiseReductionEventType ) {

		PX5DNoiseReductionEvent *ev = static_cast<PX5DNoiseReductionEvent*>(event);
		ui->noiseReductionParamName->setText( ev->getParamName() );
		ui->noiseReductionParamDial->setMinimum( ev->getMinParam() );
		ui->noiseReductionParamDial->setMaximum( ev->getMaxParam() );
		ui->noiseReductionParamDial->setValue( ev->getParameter() );

	} else {
		qDebug() << "UNKNOWN EVENT TYPE " << event->type();
	}

	event->accept();

}

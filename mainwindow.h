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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "engine/px5dcontroller.h"
#include "engine/pandoraobservable.h"
#include "engine/pandoraobserver.h"
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public PandoraObserver
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	virtual void update(PandoraObservable* o, PandoraUpdatedSignal bitflag, PandoraPreset &p);

private slots:
	void on_connectToPandora_released();
	void on_programNumber_valueChanged(int arg1);

	void on_dynamicsParamDial_valueChanged(int value);

	void on_dynamicsType_activated(int index);

	void on_programBankFactory_clicked();

	void on_programBankUser_clicked();

	void on_ampType_activated(int index);

	void on_ampParamDial1_valueChanged(int value);

	void on_ampParamDial2_valueChanged(int value);

	void on_ampParamDial3_valueChanged(int value);

	void on_ampParamDial4_valueChanged(int value);

	void on_ampParamDial5_valueChanged(int value);

	void on_cabinetType_activated(int index);

	void on_cabinetParamDial_valueChanged(int value);

	void on_modulationType_activated(int index);

	void on_modulationParamDial_valueChanged(int value);

	void on_delayType_activated(int index);

	void on_delayParamDial_valueChanged(int value);

	void on_delayParamDial2_valueChanged(int value);

	void on_reverbType_activated(int index);

	void on_reverbParamDial_valueChanged(int value);

	void on_noiseReductionParamDial_valueChanged(int value);

	void on_writeProgram_released();

	void on_actionQuit_triggered();

	void on_actionAbout_triggered();

	void on_dynamicsBox_toggled(bool arg1);

	void on_amplifierBox_toggled(bool arg1);

	void on_cabinetBox_toggled(bool arg1);

	void on_modulationBox_toggled(bool arg1);

	void on_delayBox_toggled(bool arg1);

	void on_reverbBox_toggled(bool arg1);

	void on_noiseReductionBox_toggled(bool arg1);

private:
	Ui::MainWindow *ui;
	Px5dController *m_px5dController;
	AboutDialog *m_dlgAbout;

	AboutDialog* about();
};

#endif // MAINWINDOW_H

#include "storeprogramdialog.h"
#include "ui_storeprogramdialog.h"
#include <stdio.h>
#include <QDebug>
#include "mainwindow.h"

StoreProgramDialog::StoreProgramDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::StoreProgramDialog)
{
	ui->setupUi(this);

	//FIXME: would better clean and reload in case names changed

	MainWindow *mw;
	mw = (MainWindow*)(this->parentWidget());
	if ( ui->programSlotSelect->count()==0 ) {
		for ( unsigned int i=0; i<=99; ++i ) {
			char pgName[64];
			sprintf(pgName, "#%03d: ", i);
			QString pgString(pgName);
			pgString.append( mw->getUserProgramName(i) );
			ui->programSlotSelect->addItem( pgString );
		}
	}
	int currentProgram = mw->getCurrentProgramNumber();
	ui->programSlotSelect->setCurrentIndex(currentProgram);
}

StoreProgramDialog::~StoreProgramDialog()
{
	delete ui;
}

void StoreProgramDialog::on_buttonBox_accepted()
{
	//TODO: confirm overwriting different number than default
	MainWindow *mw;
	mw = (MainWindow*)(this->parentWidget());
	int index = ui->programSlotSelect->currentIndex();
	mw->getBridge().controller()->saveCurrentProgramToSlot( index );
	this->close();
}


void StoreProgramDialog::on_buttonBox_rejected()
{
	this->close();
}

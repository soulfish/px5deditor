#ifndef STOREPROGRAMDIALOG_H
#define STOREPROGRAMDIALOG_H

#include <QDialog>

namespace Ui {
class StoreProgramDialog;
}

class StoreProgramDialog : public QDialog
{
	Q_OBJECT

public:
	explicit StoreProgramDialog(QWidget *parent = 0);
	~StoreProgramDialog();

private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();

private:
	Ui::StoreProgramDialog *ui;
};

#endif // STOREPROGRAM_H

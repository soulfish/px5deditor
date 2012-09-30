#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "constants.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

	ui->labelVersion->setText(tr("<html>"
			"<head>"
			  "<meta name=\"qrichtext\" content=\"1\" />"
			  "<style type=\"text/css\">\np, li { white-space: pre-wrap; }</style>"
			"</head>"
			"<body style=\"font-family:'Sans Serif'; font-size:12pt; font-style:normal;\">"
			  "<p style=\"margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
				"Version: %1<br/>"
				"Build date: %2<br/>"
				"Build time: %3<br/>"
				"Compiler: %4"
			  "</p>"
			"</body>"
			"</html>").arg(PGM_VERSION, BLD_DATE, BLD_TIME, CMP_VERSION));
}

AboutDialog::~AboutDialog() {
	delete ui;
}

void AboutDialog::on_pushButton_clicked() {
	this->close();
}

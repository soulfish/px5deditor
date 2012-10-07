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

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName(QString("Exp::Media"));
	QCoreApplication::setOrganizationDomain("www.exp-media.com");
	QCoreApplication::setApplicationName("PX5D Editor for Linux");
	a.setWindowIcon(QIcon(":/px5de/data/px5d_32x32.png"));

	MainWindow w;
	Qt::WindowFlags flags = w.windowFlags();
	flags |= Qt::WindowMinimizeButtonHint;
	w.setWindowFlags(flags);
	w.setFixedSize(860,512);
	w.show();

	return a.exec();
}

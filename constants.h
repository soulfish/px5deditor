#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGui/QApplication>
#include <QtCore/QString>

#define LITERAL(s) #s
#define STRINGIFY(s) LITERAL(s)
const QString PGM_VERSION(STRINGIFY(VERSION));

const QString BLD_DATE(__DATE__);
const QString BLD_TIME(__TIME__);
const QString CMP_VERSION(__VERSION__);

#endif // CONSTANTS_H

#-------------------------------------------------
#
# Project created by QtCreator 2012-09-09T18:27:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pandoraEditor
TEMPLATE = app

DEFINES += __LINUX_ALSA__

VERSION = 0.1.0-dev
DEFINES += VERSION=$$VERSION

SOURCES += main.cpp\
        mainwindow.cpp \
    engine/px5dcontroller.cpp \
    engine/RtMidi.cpp \
    engine/pandoraobserver.cpp \
    engine/pandoraobservable.cpp \
    engine/pandorapreset.cpp \
    engine/presetamp.cpp \
    engine/presetdelay.cpp \
    engine/presetdynamics.cpp \
    engine/presetmodulation.cpp \
    engine/presetnoisereduction.cpp \
    engine/presetreverb.cpp \
    engine/presetcabinet.cpp \
    aboutdialog.cpp \
    px5dbridge.cpp

HEADERS  += mainwindow.h \
    engine/px5dcontroller.h \
    engine/RtError.h \
    engine/RtMidi.h \
    engine/pandoraobserver.h \
    engine/pandoraobservable.h \
    engine/presetparameters.h \
    engine/pandorapreset.h \
    engine/presetamp.h \
    engine/presetdelay.h \
    engine/presetdynamics.h \
    engine/presetmodulation.h \
    engine/presetnoisereduction.h \
    engine/presetreverb.h \
    engine/presetcabinet.h \
    aboutdialog.h \
    constants.h \
    events.h \
    px5dbridge.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += alsa

unix:!macx:!symbian: LIBS += -lpthread

OTHER_FILES += \
    notes.txt \
    TODO.txt \
    CMakeLists.txt \
    px5de_desktop.sh

RESOURCES += \
    px5deditor.qrc

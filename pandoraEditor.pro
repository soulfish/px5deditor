#-------------------------------------------------
#
# Project created by QtCreator 2012-09-09T18:27:49
#
#-------------------------------------------------

QT       += core gui declarative

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pandoraEditor
TEMPLATE = app

DEFINES += __LINUX_ALSA__
DEFINES += DEBUG

VERSION = 0.3.0
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
    px5dbridge.cpp \
    engine/presetname.cpp \
    engine/presetreader.cpp \
    storeprogramdialog.cpp

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
    px5dbridge.h \
    engine/presetname.h \
    regexpucvalidator.h \
    engine/presetreader.h \
    storeprogramdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    storeprogramdialog.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += alsa

unix:!macx:!symbian: LIBS += -lpthread

OTHER_FILES += \
    notes.txt \
    TODO.txt \
    CMakeLists.txt \
    px5de_desktop.sh \
    px5de_man.sh \
    TODO \
    README.md \
    README \
    INSTALL \
    COPYING \
    CHANGELOG \
    AUTHORS \
    dist/PKGBUILD \
    TODO_local.txt

RESOURCES += \
    px5deditor.qrc

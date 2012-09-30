cmake_minimum_required(VERSION 2.8)

# Simple CMake project for the Korg Pandora Px5d Editor for Linux
PROJECT( px5deditor )

# note: to install in different directory:
# cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr . && make all install

# Start with detection of the kernel version
# Pandora USB-Midi required by the editor only appeared in kernel 3.0
# This does not prevent building the software, by as it won't work if
# kernel version < 3.0 we bail out with an error message.
# TODO: allow to skip this test
if ( UNIX AND NOT APPLE )
    execute_process(COMMAND uname -r
        OUTPUT_VARIABLE KERNEL_VERSION)
    string(REGEX MATCHALL "[0-9]+" KERNEL_VERSION_COMPONENTS ${KERNEL_VERSION})
    list(GET KERNEL_VERSION_COMPONENTS 0 KERNEL_MAJOR)
    list(GET KERNEL_VERSION_COMPONENTS 1 KERNEL_MINOR)
    if (KERNEL_VERSION VERSION_GREATER 3.0 OR KERNEL_VERSION VERSION_EQUAL 3.0)
        message(STATUS "Kernel Version >= 3.0, OK for Pandora USB-Midi!")
    else()
        message(FATAL_ERROR "Kernel Version < 3.0, the PX5d editor requires kernel 3.0, or the Pandora USB-Midi won't work.")
    endif()
endif()

# Various settings
set (CMAKE_COLOR_MAKEFILE ON)
set (CMAKE_CXX_FLAGS_DEBUG "-g -DQT_DEBUG -DDEBUG -D__RTMIDI_DEBUG__")
set (CMAKE_CXX_FLAGS_RELEASE "-O2 -DQT_NO_DEBUG -DNDEBUG")

# Set current version
set (VERSION_MAJOR "0")
set (VERSION_MINOR "1")
set (VERSION_PATCH "0")
set (VERSION_SUFFIX "alpha")
set (VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${VERSION_SUFFIX})
add_definitions (-DVERSION=${VERSION})

message (STATUS "Korg Pandora PX5d editor for Linux ${VERSION}
    install prefix: ${CMAKE_INSTALL_PREFIX}")

message (STATUS "Build configuration: ${CMAKE_BUILD_TYPE}")

# Sources
set( PX5DE_SRCS
  ./main.cpp
  ./mainwindow.cpp
  ./aboutdialog.cpp
  ./engine/pandoraobservable.cpp
  ./engine/presetamp.cpp
  ./engine/presetdynamics.cpp
  ./engine/RtMidi.cpp
  ./engine/presetreverb.cpp
  ./engine/presetreverb.cpp
  ./engine/pandoraobserver.cpp
  ./engine/presetcabinet.cpp
  ./engine/presetmodulation.cpp
  ./engine/px5dcontroller.cpp
  ./engine/pandorapreset.cpp
  ./engine/presetdelay.cpp
  ./engine/presetnoisereduction.cpp
)

# QT and UI stuff
set( PX5DE_MOC_HDRS
  ./mainwindow.h
  ./aboutdialog.h
)
# dialogs
set( PX5DE_UIS
  ./mainwindow.ui
  ./aboutdialog.ui
)
# resources
set( PX5DE_RCS
  ./px5deditor.qrc
)

# TODO: set -Wall only on debug builds
# add_definitions( -Wall )

# for install/uninstall
find_package(X11 REQUIRED)

# Package required: QT
FIND_PACKAGE( Qt4 REQUIRED )
if (QT4_FOUND)
    message (STATUS "Found Qt4.${QT_EDITION} version = ${QTVERSION}")
else ()
    message (FATAL_ERROR "Qt4 not found")
endif ()
SET( QT_USE_QTNETWORK TRUE )
SET( QT_USE_QTXML TRUE )

include( ${QT_USE_FILE} ${ALSA_INCLUDE_DIRS} )
QT4_ADD_RESOURCES( PX5DE_RC_SRC ${PX5DE_RCS} )
QT4_WRAP_UI( PX5DE_UI_HDRS ${PX5DE_UIS} )
QT4_WRAP_CPP( PX5DE_MOC_SRCS ${PX5DE_MOC_HDRS} )
include_directories( ${CMAKE_BINARY_DIR}   )


# Package required: Alsa and Pthread are automaticalle included with QT but not our main Lib
# so keep this bit for other tools (cli...)
find_package(Threads REQUIRED)

# Px5d editor target
ADD_EXECUTABLE( px5deditor ${PX5DE_SRCS} ${PX5DE_MOC_SRCS} ${PX5DE_RC_SRC} ${PX5DE_UI_HDRS} )
TARGET_LINK_LIBRARIES( px5deditor ${QT_LIBRARIES} ${ALSA_LIBRARIES} )

find_package (ALSA REQUIRED)
if (ALSA_FOUND)
  include_directories(${ALSA_INCLUDE_DIRS})
  target_link_libraries (px5deditor ${ALSA_LIBRARIES})
endif (ALSA_FOUND)


# Menu link and icons
if( UNIX AND NOT APPLE )

    if( NOT DESKTOP_ENTRY )
        set( DESKTOP_ENTRY px5deditor.desktop )
    endif( NOT DESKTOP_ENTRY )

    add_custom_command( OUTPUT ${DESKTOP_ENTRY}
    COMMAND touch ${DESKTOP_ENTRY}
    COMMAND sh ../px5de_desktop.sh ${CMAKE_INSTALL_PREFIX} >${DESKTOP_ENTRY}
    DEPENDS px5de_desktop.sh
    COMMENT "Generating desktop entry file"
    )

    add_custom_target( DESKTOP_ENTRY_FILE ALL
        DEPENDS ${DESKTOP_ENTRY}
    )

    set( APP_ICON data/px5deditor-logo.png )

endif( UNIX AND NOT APPLE )

# Generate manpage
if( NOT MANPAGE_DIRECTORY )
    set( MANPAGE_DIRECTORY /usr/share/man/man1 )
endif( NOT MANPAGE_DIRECTORY)

add_custom_command( OUTPUT px5deditor.1.gz
    COMMAND touch px5deditor.1
    COMMAND sh ../px5de_man.sh ${QTM_VERSION} >px5deditor.1
    COMMAND gzip -9 px5deditor.1
    DEPENDS px5de_man.sh
    COMMENT "Generating manpage"
    )

add_custom_target( MANPAGE_FILE ALL
    DEPENDS px5deditor.1.gz
)

set_directory_properties( ADDITIONAL_MAKE_CLEAN_FILES px5deditor.1 px5deditor.1.gz )


if(UNIX AND NOT APPLE)
#	install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/px5deditor DESTINATION bin )
#	install( FILES ${APP_ICON} DESTINATION share/icons )

    install (FILES data/px5d_16x16.png
             DESTINATION share/icons/hicolor/16x16/apps
             RENAME px5deditor.png)

    install (FILES data/px5d_32x32.png
             DESTINATION share/icons/hicolor/32x32/apps
             RENAME px5deditor.png)

    install (FILES data/px5d_48x48.png
             DESTINATION share/icons/hicolor/48x48/apps
             RENAME px5deditor.png)

    install (FILES data/px5d_64x64.png
             DESTINATION share/icons/hicolor/64x64/apps
             RENAME px5deditor.png)

    install (FILES data/px5d_128x128.png
             DESTINATION share/icons/hicolor/128x128/apps
             RENAME px5deditor.png)

    install( TARGETS px5deditor RUNTIME DESTINATION bin )

    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/px5deditor.desktop DESTINATION share/applications )

    install( FILES ${CMAKE_CURRENT_BINARY_DIR}/px5deditor.1.gz DESTINATION ${MANPAGE_DIRECTORY} )

endif(UNIX AND NOT APPLE)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# Packing/Distribution
# TODO: make a full feature Cpack configuration to allow building DEBs and RPMs

# Standard bzip2
SET(CPACK_GENERATOR "TBZ2")
INCLUDE(CPack)
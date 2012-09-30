Korg Pandora PX5D Editor for Linux
================================== 

!["PX5D Editor"](http://www.exp-media.com/sites/default/files/pandora/Pandora_PX5D_on_Linux.jpg "PX5D Editor")

A library and editor for the Korg Pandora PX5D on Linux.

Author:  Frédéric Jaume `<dev (a) exp-media.com>`

Website: [http://www.exp-media.com/px5d_editor_linux](http://www.exp-media.com/px5d_editor_linux)  

About
-----

The Pandora PX5D is a portable multi-effect for guitar and bass made by Korg. It
is an amazingly small and full-featured tool for guitar and bass players, has an
USB-Audio interface to use it as a sound card which works out-of-the-box on 
Linux, and a USB-MIDI interface for which support has been added in Linux 
Kernel 3.0.

Before Kernel 3.0, there was no possibility to build a preset editor for the 
device as it communicates with the computer via USB-MIDI. Since Kernel 3.0, the
USB-MIDI interface of the PX5D is detected by ALSA. A native editor became an
option instead of using the preset editor that Korg made for Windows, via Wine.

This Korg Pandora PX5D Editor for Linux is a simple, no-eye-candy preset editor
which allows to view all parameters of a preset on the PX5D and edit them in
real time.

This project is made of a small library, which can currently only be build 
with the accompanying QT GUI, but has no depency to it, and may allow to write
other editor or tools, e.g. a CLI preset dumper or batch updater.

This is a work in progress, a lot remain to be done, but preset edition is
fully functional. Have a look to the TODO file for more info on planned 
features and future ideas.

How to build
------------

The project relies on the following libraries:
- CMake (cmake) 
- QT4 (libqt4-dev) 
- ALSA (libasound2-dev)
- pthread (usually standard)
- RtMidi (bundled with the project)

which should allow easy compilation for most distributions. You may need to install ALSA and QT development
packages, as well as CMake which is used to build the application.

Download the ZIP archive from GitHub (or clone the repo), then open a terminal an go to the source 
directory you extracted from the archive, and perform the following steps:

    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/usr
    make

then as root:

    make install

if you want to remove the application, from the same directory:

    make uninstall

The `CMAKE_INSTALL_PREFIX:PATH=/usr` option may be set to your prefered directory.
`/usr/local` is the default installation location if not overriden like this, but you might
not have the menu entry set as expected on some system.

You should find "PX5E Editor for Linux" in your menu (in Multimedia, Audio or Audio/Video category),
alternatively, you can call the application directly (in a terminal) with: `px5deditor`

Usage
-----

First, ensure your PX5D is connected to USB, power it, then run the px5deditor. Once the main window is showing,
press the "Connect" button, and it should find your device an reflect it's connection status in the dedicated
status fields. You're now ready to edit your presets!

If the connection fails, enter this command in a terminal:

    aconnect -lo

This will list the devices on your computer. If you do not see "PANDORA PX5D" in this list, something
may be wrong on your system. Do you have at least Kernel 3.0 ? You can check it with this command:

    uname -r
    
If you don't have at least 3.0, you're out of luck, the PX5D cannot be used. If you do, then send me the results
of both previous commands as well as info about your distribution, which might help fix potential bugs.

Screenshot
----------

!["PX5D Editor Screenshot"](http://www.exp-media.com/sites/default/files/pandora/snapshot_20120930-alpha-small.jpeg "PX5D Editor Screenshot")


Current Status
--------------

* Library & GUI set up
* Live editing of presets works.

TODO
----

* handle saving/loading all presets to disk
* handle the drum machine
* any ideas welcome, check the full TODO file
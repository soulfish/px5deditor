#!/bin/sh
cat <<EOF
.TH px5deditor 1 "September 29, 2012" "version $1" "USER COMMANDS"
.SH NAME
px5deditor \- Preset editor for the Korg Pandora PX5D multi-effect
.SH SYNOPSIS
.B px5deditor
.SH DESCRIPTION
px5deditor is a GUI base preset editor for the guitar and bass USB device
made by Korg: the Pandora PX5d.
You can edit presets in realtime when you device is connected to your
computer.
.SH OPTIONS
px5deditor does not offer command-line options of its own; 
As a Qt application it supports Qt
options; please see:

http://doc.trolltech.com/4.2/qapplication.html#QApplication
.SH AUTHOR
Frédéric Jaume (dev (at) exp-media.com)
EOF
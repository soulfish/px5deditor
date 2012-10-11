#!/bin/sh
cat <<EOF
[Desktop Entry]
Name=PX5D Editor for Linux
Version=0.1
GenericName=Preset Editor
Comment=This is a preset editor for the Korg Pandora PX5D
Exec=$1/bin/px5deditor &
TryExec=$1/bin/px5deditor
Icon=px5deditor
Categories=Application;AudioVideo;Audio;Midi;X-Alsa;Qt
Terminal=false
Type=Application
EOF

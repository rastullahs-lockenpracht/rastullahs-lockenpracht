#!/bin/sh
#
# Creates a disk image (dmg) on Mac OS X from the command line.
# usage:
#    mkdmg.sh <volname> <srcdir>
#
# Where <volname> is the name to use for the mounted image, <vers> is the version
# number of the volume and <srcdir> is where the contents to put on the dmg are.
#
# The result will be a file called <volname>-<vers>.dmg

if [ $# != 2 ]; then
 echo "usage: mkdmg.sh volname srcdir"
 exit 0
fi

VOL="$1"
FILES="$2"

#hdiutil create "$DMG" -megabytes ${SIZE} -ov -srcfolder ${FILES} -fs "Journaled HFS+"
hdiutil create "$VOL" -ov -srcfolder ${FILES} -fs "HFS+"

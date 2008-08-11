#!/bin/sh
RASTULLAH_APP_DIR=$1
RASTULLAH_APP_NAME=$2
RASTULLAH_APP_BIN=${RASTULLAH_APP_DIR}/${RASTULLAH_APP_NAME}
CEGUI_BIN=${RASTULLAH_APP_DIR}/../Frameworks/CEGUI.framework/CEGUI

#TODO: Replace the absolute source paths to something with olist -L $RASTULLAH_APP_NAME and grep

echo Changing library installation directories for $RASTULLAH_APP_NAME

# RL Application binary
install_name_tool -change /Library/Frameworks/CEGUI.framework/Versions/A/CEGUI @executable_path/../Frameworks/CEGUI.framework/Versions/A/CEGUI $RASTULLAH_APP_BIN
install_name_tool -change libboost_system-mt-1_35.dylib @executable_path/../Frameworks/libboost_system-mt-1_35.dylib $RASTULLAH_APP_BIN
install_name_tool -change /System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/libruby.1.dylib @executable_path/../Frameworks/libruby.1.dylib $RASTULLAH_APP_BIN
install_name_tool -change /Users/blakharaz/Library/Frameworks/OgreNewt.framework/Versions/A/OgreNewt @executable_path/../Frameworks/OgreNewt.framework/Versions/A/OgreNewt $RASTULLAH_APP_BIN
install_name_tool -change /Users/blakharaz/Library/Frameworks/OpenSteer.framework/Versions/0.8/OpenSteer @executable_path/../Frameworks/OpenSteer.framework/Versions/0.8/OpenSteer $RASTULLAH_APP_BIN

# CEGUI
install_name_tool -change /Library/Frameworks/CEGUI.framework/Versions/A/CEGUI @executable_path/../Frameworks/CEGUI.framework/Versions/A/CEGUI ${CEGUI_BIN}
install_name_tool -change /Library/Frameworks/FreeType.framework/Versions/2.3/FreeType @executable_path/../Frameworks/FreeType.framework/Versions/2.3/FreeType ${CEGUI_BIN}


echo done.
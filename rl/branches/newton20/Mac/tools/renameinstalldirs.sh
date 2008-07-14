#!/bin/sh
RASTULLAH_APP_NAME=$1

#TODO: Replace the absolute source paths to something with olist -L $RASTULLAH_APP_NAME and grep

echo Changing library installation directories for $RASTULLAH_APP_NAME
install_name_tool -change /Library/Frameworks/CEGUI.framework/Versions/A/CEGUI @executable_path/../Frameworks/CEGUI.framework/Versions/A/CEGUI $RASTULLAH_APP_NAME
install_name_tool -change libboost_system-mt-1_35.dylib @executable_path/../Frameworks/libboost_system-mt-1_35.dylib $RASTULLAH_APP_NAME
install_name_tool -change /System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/libruby.1.dylib @executable_path/../Frameworks/libruby.1.dylib $RASTULLAH_APP_NAME
install_name_tool -change /Users/blakharaz/Library/Frameworks/OgreNewt.framework/Versions/A/OgreNewt @executable_path/../Frameworks/OgreNewt.framework/Versions/A/OgreNewt $RASTULLAH_APP_NAME
install_name_tool -change /Users/blakharaz/Library/Frameworks/OpenSteer.framework/Versions/0.8/OpenSteer @executable_path/../Frameworks/OpenSteer.framework/Versions/0.8/OpenSteer $RASTULLAH_APP_NAME
echo done.
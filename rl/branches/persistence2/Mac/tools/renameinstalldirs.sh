#!/bin/bash
PRODUCTS_DIR=$1
RASTULLAH_APP_DIR=${1}/${2}
RASTULLAH_APP_NAME=$3
RASTULLAH_APP_BIN=${RASTULLAH_APP_DIR}/${RASTULLAH_APP_NAME}
CEGUI_DIR=${RASTULLAH_APP_DIR}/../Frameworks/CEGUI.framework
CEGUI_BIN=${CEGUI_DIR}/CEGUI
RASTULLAH_RES_DIR=${RASTULLAH_APP_DIR}/../Resources

#TODO: Replace the absolute source paths to something with olist -L $RASTULLAH_APP_NAME and grep

echo Changing library installation directories for $RASTULLAH_APP_NAME

RL_BINARIES="$RASTULLAH_APP_BIN \
    $RASTULLAH_RES_DIR/libRlAi.dylib \
    $RASTULLAH_RES_DIR/libRlCommon.dylib \
    $RASTULLAH_RES_DIR/libRlCore.dylib \
    $RASTULLAH_RES_DIR/libRlRules.dylib \
    $RASTULLAH_RES_DIR/libRlScript.dylib \
    $RASTULLAH_RES_DIR/libRlUi.dylib \
    $RASTULLAH_RES_DIR/libboost_filesystem.dylib \
    $RASTULLAH_RES_DIR/libboost_system.dylib \
    $RASTULLAH_RES_DIR/RlScript.bundle \
    $PRODUCTS_DIR/libRlAi.dylib \
    $PRODUCTS_DIR/libRlCommon.dylib \
    $PRODUCTS_DIR/libRlCore.dylib \
    $PRODUCTS_DIR/libRlRules.dylib \
    $PRODUCTS_DIR/libRlScript.dylib \
    $PRODUCTS_DIR/libRlUi.dylib"

# RL Application binary
for cur in $RL_BINARIES 
do
    echo Processing $cur
    install_name_tool -change /Library/Frameworks/CEGUI.framework/Versions/A/CEGUI @executable_path/../Frameworks/CEGUI.framework/Versions/A/CEGUI $cur
    install_name_tool -change libboost_system-mt-1_35.dylib @executable_path/../Resources/libboost_system.dylib $cur
    install_name_tool -change ./libfmodex.dylib @executable_path/../Resources/libfmodex.dylib $cur
    install_name_tool -change libboost_filesystem-mt-1_35.dylib @executable_path/../Resources/libboost_filesystem.dylib $cur
    install_name_tool -change /System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/libruby.1.dylib @executable_path/../Frameworks/libruby.1.dylib $cur
    install_name_tool -change /Users/blakharaz/Library/Frameworks/OgreNewt.framework/Versions/A/OgreNewt @executable_path/../Frameworks/OgreNewt.framework/Versions/A/OgreNewt $cur
    install_name_tool -change /Users/blakharaz/Library/Frameworks/OpenSteer.framework/Versions/0.8/OpenSteer @executable_path/../Frameworks/OpenSteer.framework/Versions/0.8/OpenSteer $cur
    install_name_tool -change /usr/local/bin/libmeshmagick.dylib @executable_path/../Resources/libmeshmagick.dylib $cur
done

# CEGUI
install_name_tool -change /Library/Frameworks/CEGUI.framework/Versions/A/CEGUI @executable_path/../Frameworks/CEGUI.framework/Versions/A/CEGUI ${CEGUI_BIN}
install_name_tool -change /Library/Frameworks/CEGUI.framework/Versions/A/CEGUI @executable_path/../Frameworks/CEGUI.framework/Versions/A/CEGUI ${CEGUI_DIR}/Resources/CEGUIExpatParser.bundle/Contents/MacOS/CEGUIExpatParser
install_name_tool -change /Library/Frameworks/CEGUI.framework/Versions/A/CEGUI @executable_path/../Frameworks/CEGUI.framework/Versions/A/CEGUI ${CEGUI_DIR}/Resources/CEGUIFalagardWRBase.bundle/Contents/MacOS/CEGUIFalagardWRBase
install_name_tool -change /Library/Frameworks/CEGUI.framework/Versions/A/CEGUI @executable_path/../Frameworks/CEGUI.framework/Versions/A/CEGUI ${CEGUI_DIR}/Resources/CEGUIFreeImageImageCodec.bundle/Contents/MacOS/CEGUIFreeImageImageCodec
install_name_tool -change /Library/Frameworks/CEGUI.framework/Versions/A/CEGUI @executable_path/../Frameworks/CEGUI.framework/Versions/A/CEGUI ${CEGUI_DIR}/Resources/CEGUIDevILImageCodec.bundle/Contents/MacOS/CEGUIDevILImageCodec
install_name_tool -change /Library/Frameworks/FreeType.framework/Versions/2.3/FreeType @executable_path/../Frameworks/FreeType.framework/Versions/2.3/FreeType ${CEGUI_BIN}

echo done.
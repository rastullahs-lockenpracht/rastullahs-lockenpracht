# Auto-generated by EclipseNSIS Script Wizard
# 16.01.2006 15:17:43

Name "Rastullahs Lockenpracht"
SetCompressor /SOLID bzip2

# Defines
!define REGKEY "SOFTWARE\$(^Name)"
!define VERSION "Techdemo (0.1.0) [Sordul]"
!define COMPANY "Team Pantheon"
!define URL http://team-pantheon.de

# MUI defines
!define MUI_ICON "auge.ico"
!define MUI_UNICON "auge.ico"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Rastullahs Lockenpracht"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULT_FOLDER "Rastullahs Lockenpracht"
!define MUI_FINISHPAGE_SHOWREADME $INSTDIR\README.TXT

# Included files
!include Sections.nsh
!include MUI.nsh

# Reserved Files

# Variables
Var StartMenuGroup

# Installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ..\LICENSE.TXT
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuGroup
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

# Installer languages
!insertmacro MUI_LANGUAGE English

# Installer attributes
OutFile setup-engine.exe
InstallDir "$PROGRAMFILES\Rastullahs Lockenpracht"
CRCCheck on
XPStyle on
ShowInstDetails show
VIProductVersion 0.1.0.0
VIAddVersionKey ProductName "Rastullahs Lockenpracht"
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey CompanyName "${COMPANY}"
VIAddVersionKey CompanyWebsite "${URL}"
VIAddVersionKey FileVersion ""
VIAddVersionKey FileDescription ""
VIAddVersionKey LegalCopyright ""
InstallDirRegKey HKLM "${REGKEY}" Path
ShowUninstDetails show

# Installer sections
Section -Engine SEC0000
    SetOutPath $INSTDIR
    SetOverwrite on
    File ..\rl_dist\zlib1.dll
    File ..\rl_dist\boost_thread-vc80-mt-1_33.dll
    File ..\rl_dist\CEGUIBase.dll
    File ..\rl_dist\CEGUIFalagardBase.dll
    File ..\rl_dist\cg.dll
    File ..\rl_dist\devil.dll
    File ..\rl_dist\fmod.dll
    File ..\rl_dist\ilu.dll
    File ..\rl_dist\ilut.dll
    File ..\rl_dist\msvcrt-ruby18.dll
    File ..\rl_dist\Newton.dll
    File ..\rl_dist\OgreGUIRenderer.dll
    File ..\rl_dist\OgreMain.dll
    File ..\rl_dist\OgreNewt.dll
    File ..\rl_dist\OgrePlatform.dll
    File ..\rl_dist\Plugin_CgProgramManager.dll
    File ..\rl_dist\Plugin_OctreeSceneManager.dll
    File ..\rl_dist\Plugin_ParticleFX.dll
    File ..\rl_dist\Plugin_TheoraVideoSystem.dll
    File ..\rl_dist\Rastullah.exe
    File ..\rl_dist\RenderSystem_Direct3D9.dll
    File ..\rl_dist\RenderSystem_GL.dll
    File ..\rl_dist\RlCommon.dll
    File ..\rl_dist\RlCore.dll
    File ..\rl_dist\RlDialog.dll
    File ..\rl_dist\RlRules.dll
    File ..\rl_dist\RlScript.dll
    File ..\rl_dist\RlSound.dll
    File ..\rl_dist\RlUI.dll
    File ..\rl_dist\RlUiComponents.dll
    File ..\rl_dist\xerces-c_2_7.dll
    SetOutPath $INSTDIR\logs
    SetOutPath $INSTDIR
    File ..\LICENSE.TXT
    File ..\LICENSE_ARTS.TXT
    File ..\LICENSE_PROGRAM.TXT
    File ..\README.TXT
    File vccrt.msi
    ExecWait 'msiexec /i "$INSTDIR\vccrt.msi" /qn'
    Delete /REBOOTOK "$INSTDIR\vccrt.msi"
    WriteRegStr HKLM "${REGKEY}\Components" Engine 1
SectionEnd

Section -post SEC0002
    WriteRegStr HKLM "${REGKEY}" Path $INSTDIR
    WriteUninstaller $INSTDIR\uninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $SMPROGRAMS\$StartMenuGroup
    WriteIniStr "$INSTDIR\Pantheon.url" "InternetShortcut" "URL" "${URL}"
    WriteIniStr "$INSTDIR\$(^Name).url" "InternetShortcut" "URL" "${URL}/projekt"
    SetOutPath $INSTDIR    
    CreateShortCut "$SMPROGRAMS\$StartMenuGroup\$(^Name).lnk" "$INSTDIR\Rastullah.exe" "" "$INSTDIR\uninstall.exe"
    CreateShortCut "$DESKTOP\$(^Name).lnk" "$INSTDIR\Rastullah.exe" "" "$INSTDIR\uninstall.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuGroup\Pantheon im Web.lnk" "$INSTDIR\Pantheon.url" 
    CreateShortCut "$SMPROGRAMS\$StartMenuGroup\$(^Name) Website.lnk" "$INSTDIR\$(^Name).url" 
    CreateShortCut "$SMPROGRAMS\$StartMenuGroup\Uninstall $(^Name).lnk" $INSTDIR\uninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_END
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayName "$(^Name)"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayVersion "${VERSION}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" Publisher "${COMPANY}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" URLInfoAbout "${URL}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayIcon $INSTDIR\uninstall.exe
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" UninstallString $INSTDIR\uninstall.exe
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoModify 1
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoRepair 1
SectionEnd

# Macro for selecting uninstaller sections
!macro SELECT_UNSECTION SECTION_NAME UNSECTION_ID
    Push $R0
    ReadRegStr $R0 HKLM "${REGKEY}\Components" "${SECTION_NAME}"
    StrCmp $R0 1 0 next${UNSECTION_ID}
    !insertmacro SelectSection "${UNSECTION_ID}"
    Goto done${UNSECTION_ID}
next${UNSECTION_ID}:
    !insertmacro UnselectSection "${UNSECTION_ID}"
done${UNSECTION_ID}:
    Pop $R0
!macroend


Section /o un.Engine UNSEC0000
    Delete /REBOOTOK $INSTDIR\README.TXT
    Delete /REBOOTOK $INSTDIR\LICENSE_PROGRAM.TXT
    Delete /REBOOTOK $INSTDIR\LICENSE_ARTS.TXT
    Delete /REBOOTOK $INSTDIR\LICENSE.TXT
    Delete /REBOOTOK $INSTDIR\logs\*
    RMDir /REBOOTOK $INSTDIR\logs
    Delete /REBOOTOK $INSTDIR\xerces-c_2_7.dll
    Delete /REBOOTOK $INSTDIR\RlUiComponents.dll
    Delete /REBOOTOK $INSTDIR\RlUI.dll
    Delete /REBOOTOK $INSTDIR\RlSound.dll
    Delete /REBOOTOK $INSTDIR\RlScript.dll
    Delete /REBOOTOK $INSTDIR\RlRules.dll
    Delete /REBOOTOK $INSTDIR\RlDialog.dll
    Delete /REBOOTOK $INSTDIR\RlCore.dll
    Delete /REBOOTOK $INSTDIR\RlCommon.dll
    Delete /REBOOTOK $INSTDIR\RenderSystem_GL.dll
    Delete /REBOOTOK $INSTDIR\RenderSystem_Direct3D9.dll
    Delete /REBOOTOK $INSTDIR\Rastullah.exe
    Delete /REBOOTOK $INSTDIR\Plugin_TheoraVideoSystem.dll
    Delete /REBOOTOK $INSTDIR\Plugin_ParticleFX.dll
    Delete /REBOOTOK $INSTDIR\Plugin_OctreeSceneManager.dll
    Delete /REBOOTOK $INSTDIR\Plugin_CgProgramManager.dll
    Delete /REBOOTOK $INSTDIR\OgrePlatform.dll
    Delete /REBOOTOK $INSTDIR\OgreNewt.dll
    Delete /REBOOTOK $INSTDIR\OgreMain.dll
    Delete /REBOOTOK $INSTDIR\OgreGUIRenderer.dll
    Delete /REBOOTOK $INSTDIR\Newton.dll
    Delete /REBOOTOK $INSTDIR\msvcrt-ruby18.dll
    Delete /REBOOTOK $INSTDIR\ilut.dll
    Delete /REBOOTOK $INSTDIR\ilu.dll
    Delete /REBOOTOK $INSTDIR\fmod.dll
    Delete /REBOOTOK $INSTDIR\devil.dll
    Delete /REBOOTOK $INSTDIR\cg.dll
    Delete /REBOOTOK $INSTDIR\CEGUIFalagardBase.dll
    Delete /REBOOTOK $INSTDIR\CEGUIBase.dll
    Delete /REBOOTOK $INSTDIR\boost_thread-vc80-mt-1_33.dll
    Delete /REBOOTOK $INSTDIR\zlib1.dll
    DeleteRegValue HKLM "${REGKEY}\Components" Engine
SectionEnd

Section un.post UNSEC0002
    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\Uninstall $(^Name).lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\$(^Name).lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\$(^Name) Website.lnk"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\Pantheon im Web.lnk"
    Delete /REBOOTOK "$DESKTOP\$(^Name).lnk"
    Delete /REBOOTOK "$INSTDIR\$(^Name).url"
    Delete /REBOOTOK "$INSTDIR\Pantheon.url"
    Delete /REBOOTOK $INSTDIR\uninstall.exe
    DeleteRegValue HKLM "${REGKEY}" StartMenuGroup
    DeleteRegValue HKLM "${REGKEY}" Path
    DeleteRegKey /ifempty HKLM "${REGKEY}\Components"
    DeleteRegKey /ifempty HKLM "${REGKEY}"
    RMDir /REBOOTOK $SMPROGRAMS\$StartMenuGroup
    RMDir /REBOOTOK $INSTDIR
SectionEnd

# Installer functions
Function .onInit
    InitPluginsDir
FunctionEnd

# Uninstaller functions
Function un.onInit
    SetAutoClose true
    ReadRegStr $INSTDIR HKLM "${REGKEY}" Path
    ReadRegStr $StartMenuGroup HKLM "${REGKEY}" StartMenuGroup
    !insertmacro SELECT_UNSECTION Engine ${UNSEC0000}
FunctionEnd


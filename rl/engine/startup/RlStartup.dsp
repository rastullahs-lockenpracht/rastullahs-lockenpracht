# Microsoft Developer Studio Project File - Name="RlStartup" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RlStartup - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "RlStartup.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "RlStartup.mak" CFG="RlStartup - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "RlStartup - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE "RlStartup - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RlStartup - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "./bin/debug"
# PROP BASE Intermediate_Dir "./obj/debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./bin/debug"
# PROP Intermediate_Dir "./obj/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I ".\include" /I "..\core\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /GZ PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I ".\include" /I "..\core\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /GZ PRECOMP_VC7_TOBEREMOVED /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain.lib RlCore.lib /nologo /subsystem:windows /incremental:no /map /debug /machine:IX86 /out:".\bin\debug\Rastullah.exe" /pdbtype:sept /libpath:""..\core\lib\$(ConfigurationName)"" /libpath:""..\..\dependencies\ogre\OgreMain\lib\$(ConfigurationName)"" /mapinfo:exports
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain.lib RlCore.lib /nologo /subsystem:windows /incremental:no /map /debug /machine:IX86 /out:".\bin\debug\Rastullah.exe" /pdbtype:sept /libpath:""..\core\lib\Debug"" /libpath:""..\..\dependencies\ogre\OgreMain\lib\Debug"" /mapinfo:exports
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Erstelle dist
PostBuild_Cmds=$(SolutionDir)\tools\make_release.bat $(SolutionDir) $(ConfigurationName)
# End Special Build Tool

!ELSEIF  "$(CFG)" == "RlStartup - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "./bin/release"
# PROP BASE Intermediate_Dir "./obj/release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./bin/release"
# PROP Intermediate_Dir "./obj/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /O2 /I ".\include" /I "..\core\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /GF PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /O2 /I ".\include" /I "..\core\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /GF /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain.lib RlCore.lib /nologo /subsystem:windows /machine:IX86 /out:".\bin\release\Rastullah.exe" /pdbtype:sept /libpath:""..\core\lib\$(ConfigurationName)"" /libpath:""..\..\dependencies\ogre\OgreMain\lib\$(ConfigurationName)"" /opt:ref /opt:icf
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain.lib RlCore.lib /nologo /subsystem:windows /machine:IX86 /out:".\bin\release\Rastullah.exe" /pdbtype:sept /libpath:""..\core\lib\Release"" /libpath:""..\..\dependencies\ogre\OgreMain\lib\Release"" /opt:ref /opt:icf
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Erstelle dist
PostBuild_Cmds=$(SolutionDir)\tools\make_release.bat $(SolutionDir) $(ConfigurationName)
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "RlStartup - Win32 Debug"
# Name "RlStartup - Win32 Release"
# Begin Group "Quelldateien"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm"
# Begin Source File

SOURCE=.\src\RastullahMain.cpp
# End Source File
# End Group
# Begin Group "Headerdateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc"
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

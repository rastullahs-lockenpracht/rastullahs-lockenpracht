# Microsoft Developer Studio Project File - Name="RlSound" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RlSound - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "RlSound.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "RlSound.mak" CFG="RlSound - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "RlSound - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RlSound - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RlSound - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "./lib/$(ConfigurationName)"
# PROP BASE Intermediate_Dir "./obj/$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./lib/Debug"
# PROP Intermediate_Dir "./obj/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I ".\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /I "..\..\dependencies\ogre\OgreMain\Dependencies\include" /I "..\..\dependencies\openalpp\include" /I "..\..\dependencies\openal\include" /D "_WINDOWS" /D "WIN32" /D "RASTULLAH_EXPORTS" /D "_DEBUG" /D "LIBOPENAL" /D "HAVE_LIBOPENAL" /D "_MBCS" /FAs /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I ".\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /I "..\..\dependencies\ogre\OgreMain\Dependencies\include" /I "..\..\dependencies\openalpp\include" /I "..\..\dependencies\openal\include" /D "_WINDOWS" /D "WIN32" /D "RASTULLAH_EXPORTS" /D "_DEBUG" /D "LIBOPENAL" /D "HAVE_LIBOPENAL" /D "_MBCS" /FAs /GZ /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain.lib oalppd.lib /nologo /subsystem:windows /dll /incremental:no /pdb:".\lib\$(ConfigurationName)\$(ProjectName).pdb" /debug /machine:IX86 /pdbtype:sept /libpath:"..\..\dependencies\openalpp\lib" /libpath:""..\..\dependencies\ogre\OgreMain\lib\$(ConfigurationName)"" /libpath:""..\..\dependencies\ogre\PlugIns\GuiElements\bin\$(ConfigurationName)""
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain.lib oalppd.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:IX86 /pdbtype:sept /libpath:"..\..\dependencies\openalpp\lib" /libpath:""..\..\dependencies\ogre\OgreMain\lib\Debug"" /libpath:""..\..\dependencies\ogre\PlugIns\GuiElements\bin\Debug""
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RlSound - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "./lib/$(ConfigurationName)"
# PROP BASE Intermediate_Dir "./obj/$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./lib/Release"
# PROP Intermediate_Dir "./obj/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /I ".\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /I "..\..\dependencies\ogre\OgreMain\Dependencies\include" /I "..\..\dependencies\openalpp\include" /I "..\..\dependencies\openal\include" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "RASTULLAH_EXPORTS" /D "LIBOPENAL" /D "HAVE_LIBOPENAL" /D "_MBCS" /GA /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /I ".\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /I "..\..\dependencies\ogre\OgreMain\Dependencies\include" /I "..\..\dependencies\openalpp\include" /I "..\..\dependencies\openal\include" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "RASTULLAH_EXPORTS" /D "LIBOPENAL" /D "HAVE_LIBOPENAL" /D "_MBCS" /GA /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain.lib oalpp.lib /nologo /subsystem:windows /dll /machine:IX86 /pdbtype:sept /libpath:"..\..\dependencies\openalpp\lib" /libpath:""..\..\dependencies\ogre\OgreMain\lib\$(ConfigurationName)"" /libpath:""..\..\dependencies\ogre\PlugIns\GuiElements\bin\$(ConfigurationName)""
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib OgreMain.lib oalpp.lib /nologo /subsystem:windows /dll /machine:IX86 /pdbtype:sept /libpath:"..\..\dependencies\openalpp\lib" /libpath:""..\..\dependencies\ogre\OgreMain\lib\Release"" /libpath:""..\..\dependencies\ogre\PlugIns\GuiElements\bin\Release""
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RlSound - Win32 Debug"
# Name "RlSound - Win32 Release"
# Begin Group "Quelldateien"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=.\src\MusicManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SoundManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SoundResource.cpp
# End Source File
# End Group
# Begin Group "Headerdateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=.\include\MusicManager.h
# End Source File
# Begin Source File

SOURCE=.\include\SoundManager.h
# End Source File
# Begin Source File

SOURCE=.\include\SoundResource.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx"
# End Group
# End Target
# End Project

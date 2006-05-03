@SET RL_ROOT="%1"
@SET RL_DIST="%3"
@SET BUILD=%2

@SET OGREHOME="%1\Dependencies\ogrenew"
@SET OGREDEPS="%1\Dependencies\ogrenew\Dependencies"
@SET OGREPLUGINS="%1\Dependencies\ogrenew\PlugIns"
@SET OGRERENDERERS="%1\Dependencies\ogrenew\RenderSystems"
@SET OGREPLATFORMS="%1\Dependencies\ogrenew\PlatformManagers"

REM Verzeichnisse erzeugen
REM ----------------------
mkdir "%RL_DIST%"
mkdir "%RL_DIST%\logs"

REM RL kopieren
REM ----------------------
@call copy_dlls.bat %1 %2

REM Ogre Dateien kopieren
REM ---------------------
copy "%OGREHOME%\OgreMain\lib\%BUILD%\OgreMain.dll" "%RL_DIST%"
copy "%OGREHOME%\OgreMain\lib\%BUILD%\OgreMain_d.dll" "%RL_DIST%"

copy "%OGREPLUGINS%\CgProgramManager\bin\%BUILD%\Plugin_CgProgramManager.dll" "%RL_DIST%"
copy "%OGREPLUGINS%\OctreeSceneManager\bin\%BUILD%\Plugin_OctreeSceneManager.dll" "%RL_DIST%"
copy "%OGREPLUGINS%\ParticleFx\bin\%BUILD%\Plugin_ParticleFx.dll" "%RL_DIST%"

copy "%OGRERENDERERS%\Direct3D9\bin\%BUILD%\RenderSystem_Direct3D9.dll" "%RL_DIST%"
copy "%OGRERENDERERS%\GL\bin\%BUILD%\RenderSystem_GL.dll" "%RL_DIST%"

copy "%OGREPLATFORMS%\Win32\bin\%BUILD%\PlatFormManager_Win32.dll" "%RL_DIST%\OgrePlatform.dll"
copy "%OGREPLATFORMS%\Win32\bin\%BUILD%\PlatFormManager_Win32.dll" "%RL_DIST%\OgrePlatform_d.dll"

copy "%OGREHOME%\Samples\Common\CEGUIRenderer\bin\Debug\OgreGUIRenderer_d.dll" "%RL_DIST%"
copy "%OGREHOME%\Samples\Common\CEGUIRenderer\bin\Release\OgreGUIRenderer.dll" "%RL_DIST%"

copy "%OGREHOME%\Samples\Common\Bin\%BUILD%\cg.dll" "%RL_DIST%"
copy "%OGREHOME%\Samples\Common\Bin\%BUILD%\ilu.dll" "%RL_DIST%"
copy "%OGREHOME%\Samples\Common\Bin\%BUILD%\ilut.dll" "%RL_DIST%"
copy "%OGREHOME%\Samples\Common\Bin\%BUILD%\devil.dll" "%RL_DIST%"
copy "%OGREHOME%\Samples\Common\Bin\%BUILD%\zlib1.dll" "%RL_DIST%"

REM Boost Dateien kopieren
REM ---------------------
copy "%RL_ROOT%\Dependencies\boost\lib\*.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\boost\bin\*.dll" "%RL_DIST%"


REM Ruby Dateien kopieren
REM ---------------------

copy "%RL_ROOT%\Dependencies\ruby\bin\msvcrt-ruby18.dll" "%RL_DIST%"


REM Die restlichen Abhängigkeiten
REM -----------------------------
REM 
copy "%RL_ROOT%\dependencies\fmod\api\fmod.dll" "%RL_DIST%"

copy "%RL_ROOT%\Dependencies\NewtonSDK\sdk\dll\Newton.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\OgreNewt\lib\debug\OgreNewt_d.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\OgreNewt\lib\release\OgreNewt.dll" "%RL_DIST%"

copy "%RL_ROOT%\Dependencies\cegui_mk2\bin\*.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\xerces\bin\*.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\TheoraVideo\bin\%BUILD%\Plugin_TheoraVideoSystem.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\vorbis\bin\ogg.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\vorbis\bin\ogg_d.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\vorbis\bin\vorbis.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\vorbis\bin\vorbis_d.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\vorbis\bin\vorbisfile.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\vorbis\bin\vorbisfile_d.dll" "%RL_DIST%"

copy "%RL_ROOT%\Dependencies\cppunit\lib\cppunit_dll.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\cppunit\lib\cppunitd_dll.dll" "%RL_DIST%"

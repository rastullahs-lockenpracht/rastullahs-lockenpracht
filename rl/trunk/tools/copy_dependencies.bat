@SET RL_ROOT="%1"
@SET RL_DIST="%3"
@SET BUILD=%2

@SET OGREHOME="%1\Dependencies\ogrenew"
@SET OGREDEPS="%1\Dependencies\ogrenew\Dependencies"

REM Verzeichnisse erzeugen
REM ----------------------
mkdir "%RL_DIST%"
mkdir "%RL_DIST%\logs"

REM RL kopieren
REM ----------------------
@call copy_dlls.bat %1 %2

REM Ogre Dateien kopieren
REM ---------------------
copy "%OGREHOME%\lib\OgreMain.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\OgreMain_d.dll" "%RL_DIST%"

copy "%OGREHOME%\lib\Plugin_CgProgramManager.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\Plugin_OctreeSceneManager.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\Plugin_ParticleFx.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\Plugin_CgProgramManager_d.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\Plugin_OctreeSceneManager_d.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\Plugin_ParticleFx_d.dll" "%RL_DIST%"

copy "%OGREHOME%\lib\RenderSystem_Direct3D9.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\RenderSystem_GL.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\RenderSystem_Direct3D9_d.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\RenderSystem_GL_d.dll" "%RL_DIST%"

copy "%OGREHOME%\lib\OgreGUIRenderer_d.dll" "%RL_DIST%"
copy "%OGREHOME%\lib\OgreGUIRenderer.dll" "%RL_DIST%"

copy "%OGREHOME%\Samples\Common\Bin\%BUILD%\cg.dll" "%RL_DIST%"

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
copy "%RL_ROOT%\dependencies\FMOD4\api\fmodex.dll" "%RL_DIST%"

copy "%RL_ROOT%\Dependencies\NewtonSDK\sdk\dll\Newton.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\OgreNewt\lib\debug\OgreNewt_d.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\OgreNewt\lib\release\OgreNewt.dll" "%RL_DIST%"

copy "%RL_ROOT%\Dependencies\cegui_mk2\bin\*.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\xerces\bin\*.dll" "%RL_DIST%"

copy "%RL_ROOT%\Dependencies\ois\dll\OIS.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\ois\dll\OIS_d.dll" "%RL_DIST%"

copy "%RL_ROOT%\Dependencies\cppunit\lib\cppunit_dll.dll" "%RL_DIST%"
copy "%RL_ROOT%\Dependencies\cppunit\lib\cppunitd_dll.dll" "%RL_DIST%"

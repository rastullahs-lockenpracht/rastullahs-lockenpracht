@SET RL_ROOT=%1
@SET RL_DIST=%1\rl_dist
@SET BUILD=%2

@SET OGREHOME=%RL_ROOT%\Dependencies\ogre
@SET OGREDEPS=%OGREHOME%\Dependencies
@SET OGREPLUGINS=%OGREHOME%\PlugIns
@SET OGRERENDERERS=%OGREHOME%\RenderSystems
@SET OGREPLATFORMS=%OGREHOME%\PlatformManagers

REM Verzeichnisse erzeugen
REM ----------------------
mkdir %RL_DIST%
mkdir %RL_DIST%\logs

REM RL kopieren
REM ----------------------
@call copy_dlls.bat %1 %2

REM Ogre Dateien kopieren
REM ---------------------
copy %OGREHOME%\OgreMain\lib\%BUILD%\OgreMain.dll %RL_DIST%
copy %OGREHOME%\OgreMain\lib\%BUILD%\OgreMain_d.dll %RL_DIST%
copy %OGREHOME%\OgreOde\lib\%BUILD%\OgreOde_Core.dll %RL_DIST%
copy %OGREHOME%\OgreOde\lib\%BUILD%\OgreOde_Core_d.dll %RL_DIST%

copy %OGREPLUGINS%\CgProgramManager\bin\%BUILD%\Plugin_CgProgramManager.dll %RL_DIST%
copy %OGREPLUGINS%\OctreeSceneManager\bin\%BUILD%\Plugin_OctreeSceneManager.dll %RL_DIST%
copy %OGREPLUGINS%\ParticleFx\bin\%BUILD%\Plugin_ParticleFx.dll %RL_DIST%
copy %OGRERENDERERS%\Direct3D9\bin\%BUILD%\RenderSystem_Direct3D9.dll %RL_DIST%
copy %OGRERENDERERS%\GL\bin\%BUILD%\RenderSystem_GL.dll %RL_DIST%

copy %OGREPLATFORMS%\Win32\bin\%BUILD%\PlatFormManager_Win32.dll %RL_DIST%\OgrePlatform.dll
copy %OGREPLATFORMS%\Win32\bin\%BUILD%\PlatFormManager_Win32.dll %RL_DIST%\OgrePlatform_d.dll

copy %OGREHOME%\Samples\Common\CEGUIRenderer\bin\Debug\OgreGUIRenderer_d.dll %RL_DIST%
copy %OGREHOME%\Samples\Common\CEGUIRenderer\bin\Release\OgreGUIRenderer.dll %RL_DIST%

copy %OGREDEPS%\%BUILD%\lib\cg.dll %RL_DIST%
copy %OGREDEPS%\%BUILD%\lib\ilu.dll %RL_DIST%
copy %OGREDEPS%\%BUILD%\lib\ilut.dll %RL_DIST%
copy %OGREDEPS%\%BUILD%\lib\devil.dll %RL_DIST%
copy %OGREDEPS%\%BUILD%\lib\zlib1.dll %RL_DIST%

copy %RL_ROOT%\Dependencies\cegui\bin\CEGUIBase.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\cegui\bin\CEGUIBase_d.dll %RL_DIST%

copy %RL_ROOT%\Dependencies\cegui\dependencies\lib\xerces-c_2_5_0.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\cegui\dependencies\lib\xerces-c_2_5_0D.dll %RL_DIST%

REM Ruby Dateien kopieren
REM ---------------------

copy %RL_ROOT%\Dependencies\Python\Python23.dll %RL_DIST%

REM Die restlichen Abhängigkeiten
REM -----------------------------
REM 

copy %RL_ROOT%\Dependencies\openal\dll\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\boost\bin\*.dll %RL_DIST%

pause



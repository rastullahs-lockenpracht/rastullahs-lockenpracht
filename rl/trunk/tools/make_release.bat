@SET RL_ROOT=%1
@SET RL_DIST=%1\rl_dist
@SET BUILD=%2

@SET OGREHOME=%RL_ROOT%\Dependencies\ogre
@SET OGREADDONS=%RL_ROOT%\Dependencies\ogreaddons
@SET OGREDEPS=%OGREHOME%\Dependencies
@SET OGREPLUGINS=%OGREHOME%\PlugIns
@SET OGRERENDERERS=%OGREHOME%\RenderSystems
@SET OGREPLATFORMS=%OGREHOME%\PlatformManagers

REM Verzeichnisse erzeugen
REM ----------------------
mkdir %RL_DIST%
mkdir %RL_DIST%\logs

REM Ogre Dateien kopieren
REM ---------------------
copy %OGREHOME%\OgreMain\lib\%BUILD%\OgreMain.dll %RL_DIST%
copy %OGREHOME%\OgreMain\lib\%BUILD%\OgreMain_d.dll %RL_DIST%
copy %OGREHOME%\OgreOde\lib\%BUILD%\OgreOde.dll %RL_DIST%
copy %OGREHOME%\OgreOde\lib\%BUILD%\OgreOde_d.dll %RL_DIST%

copy %OGREPLUGINS%\NatureSceneManager\bin\%BUILD%\Plugin_NatureSceneManager.dll %RL_DIST%
copy %OGREPLUGINS%\BSPSceneManager\bin\%BUILD%\Plugin_BSPSceneManager.dll %RL_DIST%
copy %OGREPLUGINS%\CgProgramManager\bin\%BUILD%\Plugin_CgProgramManager.dll %RL_DIST%
copy %OGREPLUGINS%\OctreeSceneManager\bin\%BUILD%\Plugin_OctreeSceneManager.dll %RL_DIST%
copy %OGREPLUGINS%\FileSystem\bin\%BUILD%\Plugin_FileSystem.dll %RL_DIST%
copy %OGREPLUGINS%\ParticleFx\bin\%BUILD%\Plugin_ParticleFx.dll %RL_DIST%
copy %OGRERENDERERS%\Direct3D9\bin\%BUILD%\RenderSystem_Direct3D9.dll %RL_DIST%
copy %OGRERENDERERS%\GL\bin\%BUILD%\RenderSystem_GL.dll %RL_DIST%

copy %OGREPLATFORMS%\Win32\bin\%BUILD%\PlatFormManager_Win32.dll %RL_DIST%
copy %OGREPLATFORMS%\Win32\bin\%BUILD%\PlatFormManager_Win32.dll %RL_DIST%\OgrePlatform.dll

xcopy %OGREDEPS%\DevIL\lib\*.dll %RL_DIST%\ /I /K /Y /F

REM Ruby Dateien kopieren
REM ---------------------

copy %RL_ROOT%\Dependencies\ruby\bin\msvcrt-ruby18.dll %RL_DIST%

REM Die restlichen Abhängigkeiten
REM -----------------------------
REM 

copy %RL_ROOT%\Dependencies\openal\dll\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\openalpp\bin\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\openthreads\bin\win32\*.dll %RL_DIST%

copy %RL_ROOT%\Dependencies\ode\lib\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\boost\lib\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\xerces\bin\*.dll %RL_DIST%

copy %RL_ROOT%\Dependencies\cegui\bin\CEGUIBase.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\cegui\bin\OgreGUIRenderer.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\cegui\bin\CEGUITaharezLook.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\cegui\bin\CEGUIBase_d.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\cegui\bin\OgreGUIRenderer_d.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\cegui\bin\CEGUITaharezLook_d.dll %RL_DIST%

pause



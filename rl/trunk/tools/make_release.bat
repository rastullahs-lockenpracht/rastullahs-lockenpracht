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
mkdir %RL_DIST%\media
mkdir %RL_DIST%\script

REM Binaries kopieren
REM ---------------------
copy %RL_ROOT%\engine\startup\bin\%BUILD%\Rastullah.exe %RL_DIST%\
xcopy %RL_ROOT%\engine\startup\xml\*.* %RL_DIST%\ /I /K /Y /F
copy %RL_ROOT%\engine\core\lib\%BUILD%\RlCore.dll %RL_DIST%\
xcopy %RL_ROOT%\engine\core\scripts\*.* %RL_DIST%\script\ /I /K /Y /F
copy %RL_ROOT%\engine\rules\lib\%BUILD%\RlRules.dll %RL_DIST%\
copy %RL_ROOT%\engine\sound\lib\%BUILD%\RlSound.dll %RL_DIST%\
copy %RL_ROOT%\engine\ui\lib\%BUILD%\RlUi.dll %RL_DIST%\

REM Daten kopieren
REM ---------------------
copy %RL_ROOT%\engine\rules\data\* %RL_DIST%\

REM Media kopieren
REM ---------------------
copy %RL_ROOT%\engine\rules\basis.xdi %RL_DIST%\
copy %RL_ROOT%\modules\common\OgreCore.zip %RL_DIST%\media\
xcopy %RL_ROOT%\modules\common\conf\*.* %RL_DIST%\ /I /K /Y /F
xcopy %RL_ROOT%\modules\common\fonts\*.** %RL_DIST%\ /I /K /Y /F
xcopy %RL_ROOT%\modules\common\gui\*.* %RL_DIST%\ /I /K /Y /F
xcopy %RL_ROOT%\modules\common\gui\media\*.* %RL_DIST%\media\ /I /K /Y /F
xcopy %RL_ROOT%\modules\common\sound\*.* %RL_DIST%\media\ /I /K /Y /F
xcopy %RL_ROOT%\modules\common\models\*.* %RL_DIST%\media\ /I /K /Y /F
xcopy %RL_ROOT%\modules\common\materials\*.* %RL_DIST%\media\ /I /K /Y /F

xcopy %RL_ROOT%\modules\minidemo\conf\*.* %RL_DIST%\ /I /K /Y /F
xcopy %RL_ROOT%\modules\minidemo\maps\*.* %RL_DIST%\media\ /I /K /Y /F
xcopy %RL_ROOT%\modules\minidemo\models\*.* %RL_DIST%\media\ /I /K /Y /F
xcopy %RL_ROOT%\modules\minidemo\sound\*.* %RL_DIST%\media\ /I /K /Y /F
xcopy %RL_ROOT%\modules\minidemo\materials\*.* %RL_DIST%\media\ /I /K /Y /F

xcopy %RL_ROOT%\modules\minidemo\scripts\*.* %RL_DIST%\script\ /I /K /Y /F

REM Ogre Dateien kopieren
REM ---------------------
copy %OGREHOME%\OgreMain\lib\%BUILD%\OgreMain.dll %RL_DIST%
copy %OGREHOME%\OgreMain\lib\%BUILD%\OgreMain_d.dll %RL_DIST%

copy %OGREPLUGINS%\NatureSceneManager\bin\%BUILD%\Plugin_NatureSceneManager.dll %RL_DIST%
copy %OGREPLUGINS%\BSPSceneManager\bin\%BUILD%\Plugin_BSPSceneManager.dll %RL_DIST%
copy %OGREPLUGINS%\CgProgramManager\bin\%BUILD%\Plugin_CgProgramManager.dll %RL_DIST%
copy %OGREPLUGINS%\OctreeSceneManager\bin\%BUILD%\Plugin_OctreeSceneManager.dll %RL_DIST%
copy %OGREPLUGINS%\FileSystem\bin\%BUILD%\Plugin_FileSystem.dll %RL_DIST%
copy %OGREPLUGINS%\ParticleFx\bin\%BUILD%\Plugin_ParticleFx.dll %RL_DIST%
rem copy %OGREPLUGINS%\GuiElements\bin\%BUILD%\Plugin_GuiElements.dll %RL_DIST%

rem copy %OGRERENDERERS%\Direct3D7\bin\%BUILD%\RenderSystem_Direct3D7.dll %RL_DIST%
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

REM TODO mit IFDEF oder sowas zwischen Debug und Release unterscheiden.
REM copy %RL_ROOT%\Dependencies\openalpp\bin\openalpp.dll %RL_DIST%
REM copy %RL_ROOT%\Dependencies\openthreads\bin\win32\OpenThreadsWin32.dll %RL_DIST%
REM copy %RL_ROOT%\Dependencies\vorbis\bin\ogg.dll %RL_DIST%
REM copy %RL_ROOT%\Dependencies\vorbis\bin\vorbis.dll %RL_DIST%
REM copy %RL_ROOT%\Dependencies\vorbis\bin\vorbisenc.dll %RL_DIST%
REM copy %RL_ROOT%\Dependencies\vorbis\bin\vorbisfile.dll %RL_DIST%

copy %RL_ROOT%\Dependencies\openal\dll\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\openalpp\bin\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\openthreads\bin\win32\*.dll %RL_DIST%

REM copy %RL_ROOT%\Dependencies\vorbis\bin\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\ode\lib\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\boost\lib\*.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\xerces\bin\*.dll %RL_DIST%

copy %RL_ROOT%\Dependencies\cegui\bin\win32\%BUILD%\CEGUIBase.dll %RL_DIST%
copy %RL_ROOT%\Dependencies\cegui\bin\win32\%BUILD%\OgreGUIRenderer.dll %RL_DIST%
copy "%RL_ROOT%\Dependencies\cegui\bin\win32\%BUILD%\Taharez Look.dll" %RL_DIST%

call %RL_ROOT%\tools\make_release_cegui.bat %RL_ROOT% %RL_DIST% %BUILD%

pause



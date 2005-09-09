@SET RL_ROOT=%1
@SET BUILD=%2
@SET TARGET_DIR=%1\engine\rules\tests\%BUILD%

@SET OGREHOME=%RL_ROOT%\Dependencies\ogre
@SET OGREDEPS=%OGREHOME%\Dependencies

REM Binaries kopieren
REM ---------------------
copy %RL_ROOT%\engine\rules\lib\%BUILD%\RlRules.dll %TARGET_DIR%\

REM Ogre-Dateien kopieren
REM ---------------------
copy %OGREHOME%\OgreMain\lib\%BUILD%\OgreMain.dll %TARGET_DIR%
xcopy %OGREDEPS%\DevIL\lib\*.dll %TARGET_DIR%\ /I /K /Y /F

REM Ruby-Dateien kopieren
REM ---------------------
@REM copy %RL_ROOT%\Dependencies\ruby\bin\msvcrt-ruby18.dll %TARGET_DIR%

REM Daten-Dateien kopieren
REM ---------------------
copy %RL_ROOT%\engine\rules\data\* %TARGET_DIR%

pause


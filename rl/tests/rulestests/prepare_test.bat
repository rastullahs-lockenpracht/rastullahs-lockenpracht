@echo off
set RL_ROOT=%1..
set BUILD=%2
set TARGET_DIR=tests\rulestests\%BUILD%

set DEPENDENCIES=%RL_ROOT%dependencies
set OGREHOME=%DEPENDENCIES%\ogrenew
set OGREDEPS=%OGREHOME%\Dependencies

call prepare_test_dlls.bat

cd %1
REM Binaries kopieren
REM ---------------------
copy %RL_ROOT%\engine\common\lib\%BUILD%\RlCommon.dll	%TARGET_DIR%\
copy %RL_ROOT%\engine\core\lib\%BUILD%\RlCore.dll	%TARGET_DIR%\
copy %RL_ROOT%\engine\sound\lib\%BUILD%\RlSound.dll	%TARGET_DIR%\
copy %RL_ROOT%\engine\rules\lib\%BUILD%\RlRules.dll	%TARGET_DIR%\

REM Dependencies kopieren
REM ---------------------
copy %DEPENDENCIES%\cppunit\lib\*.dll			%TARGET_DIR%
call %RL_ROOT%\tools\copy_dependencies.bat %1 %2	%TARGET_DIR%


REM Dependencies kopieren
REM ---------------------
REM /* copy %DEPENDENCIES%\ruby\bin\msvcrt-ruby18.dll %TARGET_DIR% */
copy %DEPENDENCIES%\fmod\api\fmod.dll			%TARGET_DIR%

REM Testdaten-Dateien kopieren
REM ---------------------
copy %RL_ROOT%\engine\rules\data\*			%TARGET_DIR%

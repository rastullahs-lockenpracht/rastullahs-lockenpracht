@echo off
set RL_ROOT=%1..
set BUILD=%2
set TARGET_DIR=%1\tests\rulestests\%BUILD%

set DEPENDENCIES=%RL_ROOT%dependencies
set OGREHOME=%DEPENDENCIES%\ogrenew
set OGREDEPS=%OGREHOME%\Dependencies

REM Binaries kopieren
REM ---------------------
cd %1
copy ..\engine\common\lib\%BUILD%\RlCommon.dll	%TARGET_DIR%\
echo copy %RL_ROOT%\engine\common\lib\%BUILD%\RlCommon.dll	%TARGET_DIR%\
copy ..\engine\core\lib\%BUILD%\RlCore.dll	%TARGET_DIR%\
copy ..\engine\sound\lib\%BUILD%\RlSound.dll	%TARGET_DIR%\
copy ..\engine\rules\lib\%BUILD%\RlRules.dll	%TARGET_DIR%\

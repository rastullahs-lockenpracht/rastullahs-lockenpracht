@SET RL_ROOT=%1
@SET RL_DIST=%1\rl_dist
@SET BUILD=%2

REM Verzeichnisse erzeugen
REM ----------------------
mkdir %RL_DIST%
mkdir %RL_DIST%\media
mkdir %RL_DIST%\script

REM Binaries kopieren
REM ---------------------
copy %RL_ROOT%\engine\startup\bin\%BUILD%\Rastullah.exe %RL_DIST%\
copy %RL_ROOT%\engine\core\lib\%BUILD%\RlCore.dll %RL_DIST%\
copy %RL_ROOT%\engine\rules\lib\%BUILD%\RlRules.dll %RL_DIST%\
copy %RL_ROOT%\engine\sound\lib\%BUILD%\RlSound.dll %RL_DIST%\
copy %RL_ROOT%\engine\ui\lib\%BUILD%\RlUi.dll %RL_DIST%\
copy %RL_ROOT%\engine\common\lib\%BUILD%\RlCommon.dll %RL_DIST%\

pause



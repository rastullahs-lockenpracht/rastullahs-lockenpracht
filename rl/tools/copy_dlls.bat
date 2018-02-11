@SET RL_ROOT="%1"
@SET RL_DIST="%1\rl_dist"
@SET BUILD="%2"

REM Verzeichnisse erzeugen
REM ----------------------
mkdir "%RL_DIST%"
mkdir "%RL_DIST%\logs"

REM Binaries kopieren
REM ---------------------
copy "%RL_ROOT%\engine\common\lib\%BUILD%\RlCommon.dll" "%RL_DIST%"
copy "%RL_ROOT%\engine\core\lib\%BUILD%\RlCore.dll" "%RL_DIST%"
copy "%RL_ROOT%\engine\dialog\lib\%BUILD%\RlDialog.dll" "%RL_DIST%"
copy "%RL_ROOT%\engine\rules\lib\%BUILD%\RlRules.dll" "%RL_DIST%"
copy "%RL_ROOT%\engine\script\lib\%BUILD%\RlScript.dll" "%RL_DIST%"
copy "%RL_ROOT%\engine\startup\bin\%BUILD%\Rastullah.exe" "%RL_DIST%"
copy "%RL_ROOT%\engine\ui\lib\%BUILD%\RlUi.dll" "%RL_DIST%"
copy "%RL_ROOT%\engine\uicomponents\lib\%BUILD%\RlUiComponents.dll" "%RL_DIST%"
copy "%RL_ROOT%\engine\ai\lib\%BUILD%\RlAI.dll" "%RL_DIST%"

%module(directors="1") RlScript

%include "RlSound.head.inc"
%include "RlCore.head.inc"
%include "RlRules.head.inc"
%include "RlUi.head.inc"


%include "TypeMaps.i"
%include "stl.i"
%include "std_string.i"

// Kopie. Falls das nochmal irgendwohin kopiert werden muss,
// In separate Datei auslagen.
#if !defined(RL_LONGLONG)
#   if (_MSC_VER >= 1200) && (_MSC_VER < 1300)
#       define RL_LONGLONG _int64
#   else
#       define RL_LONGLONG long long
#   endif
#endif

%include "RlSound.inc"
%include "RlCore.inc"
%include "RlRules.inc"
%include "RlUi.inc"
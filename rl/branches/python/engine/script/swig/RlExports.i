/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

%module(directors="1") RlScript

%{
#undef min
%}

%include "RlUi.head.inc"
%include "RlSound.head.inc"
%include "RlCore.head.inc"
%include "RlRules.head.inc"
%include "RlScript.head.inc"


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

%feature("director:except") {
	Throw(rl::RuntimeException, StringValuePtr($error) );
}

namespace Swig {
	class DirectorException;
}

%exception {
  try {
    $action
  }
  catch (Ogre::Exception& oe) {
    PyErr_SetString(PyErr_RuntimeException, oe.getFullDescription().c_str());
  }
  catch (rl::Exception& re ) {
    PyErr_SetString(PyErr_RuntimeException, re.toString().c_str());
  }
  catch (Swig::DirectorException &e) { 
	SWIG_fail; 
  }
}

%{
#pragma warning( disable : 4101 )	// deaktiviere Warnung ueber unreferenzierte lokale Variable, 
									// da dies in allen erzeugten Exceptionhandlern auftritt
%}

%include "RlSound.inc"
%include "RlCore.inc"
%include "RlRules.inc"
%include "RlUi.inc"
%include "RlScript.inc"


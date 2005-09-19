/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

%module(directors="1") RlScript

%{
#undef min
%}

%include "RlCommon.head.inc"
%include "RlUi.head.inc"
%include "RlMultimedia.head.inc"
%include "RlCore.head.inc"
%include "RlRules.head.inc"
%include "RlDialog.head.inc"
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
	$depth--;
	int status = -1;
    rb_eval_string_protect("print $!", &status);
	Throw(rl::ScriptInvocationFailedException, string("Es ist eine Ausnahme in Ruby aufgetreten. ") + StringValuePtr($error) );
}

namespace Swig {
	class DirectorException;
}

%exception {
  try {
    $action
  }
  catch (Ogre::Exception& oe) {
    static VALUE ogreException = rb_define_class("OgreException", rb_eStandardError);
    rb_raise(ogreException, oe.getFullDescription().c_str());
  }
  catch (rl::Exception& re ) {
    static VALUE rlException = rb_define_class("RlException", rb_eStandardError);
    rb_raise(rlException, re.toString().c_str());
  }
  catch (Swig::DirectorException&) {
    static VALUE rlException = rb_define_class("DirectorException", rb_eStandardError);
    rb_raise(rlException, "Eine Director Exception ist aufgetreten");
  } 
}

%{
#pragma warning( disable : 4101 )	// deaktiviere Warnung ueber unreferenzierte lokale Variable, 
									// da dies in allen erzeugten Exceptionhandlern auftritt
%}

%include "RlMultimedia.inc"
%include "RlCore.inc"
%include "RlRules.inc"
%include "RlDialog.inc"
%include "RlUi.inc"
%include "RlScript.inc"


/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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

// Activate Directors and name the module RlScript
%module(directors="1") RlScript
/* Tell SWIG to keep track of mappings between C/C++ structs/classes. */
%trackobjects;

%{
#undef min

#if defined(_MSC_VER) && (_MSC_VER > 1300)   // MS Visual Studio 2005
// Sichere Versionen der Stringfunktionen benutzen
#	if !defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
#		define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#	endif
#endif

// Has to be included before Ogre, MemoryManager issues...
#include <xercesc/util/XMemory.hpp>	

// Exporting the function for getting ruby instances
VALUE RL_RubyInstanceFor(void* ptr) 
{
	return SWIG_RubyInstanceFor(ptr);
}
// Exporting the function for removing ruby instances
void RL_RubyRemoveTracking(void* ptr) 
{
	return SWIG_RubyRemoveTracking(ptr);
}

#include "FixRubyHeaders.h"

#include <CEGUIExceptions.h>

%}

%include "stl.i"
%include "std_string.i"
%include "std_pair.i"

// Header includes
%include "RlCommon.head.swig"
%include "RlUi.head.swig"
%include "RlCore.head.swig"
%include "RlRules.head.swig"
%include "RlDialog.head.swig"
%include "RlAi.head.swig"
%include "RlScript.head.swig"

%include "TypeMaps.i"






// Kopie. Falls das nochmal irgendwohin kopiert werden muss,
// In separate Datei auslagen.
#if !defined(RL_LONGLONG)
#   if (_MSC_VER >= 1200) && (_MSC_VER < 1300)
#       define RL_LONGLONG _int64
#   else
#       define RL_LONGLONG long long
#   endif
#endif




// Handling of errors in director methods
%feature("director:except") 
{
	RL_handleRubyError( error );
	return Qnil;
}

%{
// Error Handling for Ruby
void RL_handleRubyError( VALUE error )
{
	std::stringstream stream;	
	// get error class
    VALUE klass = rb_class_path(CLASS_OF(error));
    stream << RSTRING(klass)->ptr << " (\""; 

    // get error message
    VALUE message = rb_obj_as_string(error);
    stream << RSTRING(message)->ptr << "\"), ";

    // get backtrace
    if(!NIL_P(ruby_errinfo)) 
    {
		stream << "Callstack: [ ";
        VALUE ary = rb_funcall(
            ruby_errinfo, rb_intern("backtrace"), 0);
        int c;
        for (c=RARRAY(ary)->len; c>0; c--) {     
            stream <<  RSTRING(RARRAY(ary)->ptr[c-1])->ptr;
            if( c > 1 )
				stream << ", "; 
        }
        stream << "]";
    }
    else
		stream << "[ No Callstack found ]";
     
    LOG_ERROR(rl::Logger::SCRIPT, stream.str() );
    rl::WindowFactory::getSingleton().writeToConsole( stream.str() );  
}
%}



// Converting C++ Exceptions to Ruby Exceptions
%exception %{
  try {
    $action
  }
  catch (rl::Exception& re ) {
    static VALUE rlException = rb_define_class("RlException", rb_eRuntimeError);
    rb_raise(rlException, re.toString().c_str());
  }
  catch (CEGUI::Exception& ce) {
    static VALUE ceguiException = rb_define_class("CeguiException", rb_eRuntimeError);
    rb_raise(ceguiException, ce.getMessage().c_str());
  }
  catch (std::exception& se) {
    static VALUE stdException = rb_define_class("StdException", rb_eRuntimeError);
    rb_raise(stdException, se.what());
  }
  catch (...) {
    static VALUE unknownException = rb_define_class("UnknownException", rb_eRuntimeError);
    rb_raise(unknownException,"Unbekannte Exception");
  }
%}



// deaktiviere Warnung ueber unreferenzierte lokale Variable, 
// da dies in allen erzeugten Exceptionhandlern auftritt
%{
#pragma warning( disable : 4101 )									
%}

// Include bodies

%include "RlCommon.swig"
%include "RlCore.swig"
%include "RlRules.swig"
%include "RlDialog.swig"
%include "RlUi.swig"
%include "RlAi.swig"
%include "RlScript.swig"


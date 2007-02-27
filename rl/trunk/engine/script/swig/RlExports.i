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

#include <xercesc/util/XMemory.hpp>	// Muss vor Ogre stehen (zumindest fuer VS)

// Exportieren der Ruby Instanz Rückgabe
VALUE RL_RubyInstanceFor(void* ptr) 
{
	return SWIG_RubyInstanceFor(ptr);
}
void RL_RubyRemoveTracking(void* ptr) 
{
	return SWIG_RubyRemoveTracking(ptr);
}
%}


%include "RlCommon.head.swig"
%include "RlUi.head.swig"
%include "RlCore.head.swig"
%include "RlRules.head.swig"
%include "RlDialog.head.swig"
%include "RlAi.head.swig"
%include "RlScript.head.swig"



%include "TypeMaps.i"
%include "stl.i"
%include "std_string.i"
%include "std_pair.i"

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
	stringstream stream;	
	VALUE lasterr = rb_gv_get("$!");

    // class
    VALUE klass = rb_class_path(CLASS_OF(lasterr));
    stream << RSTRING(klass)->ptr << " ("; 

    // message
    VALUE message = rb_obj_as_string(lasterr);
    stream << RSTRING(message)->ptr << ") " << endl;

    // backtrace
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
        
    rl::WindowFactory::getSingleton().writeToConsole( stream.str() );        
	Throw(rl::ScriptInvocationFailedException, stream.str() );
}

namespace Swig {
	class DirectorException;
}

%exception {
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
  catch (Ogre::Exception& oe) {
    static VALUE ogreException = rb_define_class("OgreException", rb_eRuntimeError);
    rb_raise(ogreException, oe.getFullDescription().c_str());
  }
  catch (Swig::DirectorException&) {
    static VALUE directorException = rb_define_class("DirectorException", rb_eRuntimeError);
    rb_raise(directorException, "Eine Director Exception ist aufgetreten");
  } 
  catch (std::exception& se) {
    static VALUE stdException = rb_define_class("StdException", rb_eRuntimeError);
    rb_raise(stdException, se.what());
  }
  catch (...) {
    static VALUE unknownException = rb_define_class("UnknownException", rb_eRuntimeError);
    rb_raise(unknownException,"Unbekannte Exception");
  }
}

%{
#pragma warning( disable : 4101 )	// deaktiviere Warnung ueber unreferenzierte lokale Variable, 
									// da dies in allen erzeugten Exceptionhandlern auftritt
%}

// doWithActor( Actor* ) oder andere Director-Methoden Parameter
%typemap(directorin) SWIGTYPE*
{
	// Auf Director testen
	Swig::Director *resultdirector = 0;
	resultdirector = dynamic_cast<Swig::Director *>($1);
    if (resultdirector) {
        $input = resultdirector->swig_get_self();
    }
    else
	{
		VALUE val = SWIG_RubyInstanceFor( $1 );
		
		// Es gab das SkriptObjekt noch nicht
		if( val == Qnil )
		{
			$input = SWIG_NewPointerObj((void *) $1, $1_descriptor, NULL);		
		}
		else	
			$input = val;
	}
} 
    
// Actor* getActor oder andere OUTPUT Parameter
%typemap(out) SWIGTYPE*
{
	VALUE val = SWIG_RubyInstanceFor( $1 );
	
	// Es gab das SkriptObjekt noch nicht
	if( val == Qnil )
	{
		$result = SWIG_NewPointerObj((void *) $1, $1_descriptor, NULL);			
	}
	else	
		$result = val;
} 

// Animation* getActor oder andere OUTPUT Parameter für DYNAMICs
%typemap(out) SWIGTYPE* DYNAMIC, SWIGTYPE& DYNAMIC
{
	VALUE val = SWIG_RubyInstanceFor( $1 );
	
	// Es gab das SkriptObjekt noch nicht
	if( val == Qnil )
	{
		// Dynamic Cast ausführen
		swig_type_info *ty = SWIG_TypeDynamicCast($1_descriptor, (void **) &$1);
		$result = SWIG_NewPointerObj((void *) $1, ty, 0);		
	}
	else	
		$result = val;
} 

// doWithAnimation( Animation* ) oder andere Director-Methoden Parameter für DYNAMICs
%typemap(directorin) SWIGTYPE* DYNAMIC, SWIGTYPE& DYNAMIC
{
	// Auf Director testen
	Swig::Director *resultdirector = NULL;
	resultdirector = dynamic_cast<Swig::Director *>($1);
    if (resultdirector) {
        $input = resultdirector->swig_get_self();        	
    }
    else
	{
		VALUE val = SWIG_RubyInstanceFor( $1 );
		
		// Es gab das SkriptObjekt noch nicht
		if( val == Qnil )
		{
			// Dynamic Cast ausführen
			swig_type_info *ty = SWIG_TypeDynamicCast($1_descriptor, (void **) &$1);
			$input = SWIG_NewPointerObj((void *) $1, ty, 0);		
		}
		else	
			$input = val;
	}
} 

%include "RlCommon.swig"
%include "RlCore.swig"
%include "RlRules.swig"
%include "RlDialog.swig"
%include "RlUi.swig"
%include "RlAi.swig"
%include "RlScript.swig"


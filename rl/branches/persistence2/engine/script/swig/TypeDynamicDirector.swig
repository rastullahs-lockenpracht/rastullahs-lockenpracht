/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

/* Typemaps for all Dynamic or Director Types
 * These typemaps ensure no C++ object 
 * will have links to more than one Ruby objecz
 * 
 */

%{
static VALUE getInScriptObject( void* ptr, swig_type_info *type )
{
    VALUE val = SWIG_RubyInstanceFor( ptr );
	
	// Es gab das SkriptObjekt noch nicht
	if( NIL_P(val) ) 
		return SWIG_NewPointerObj(ptr, type, 0);			
	else	
		return val;
}
%}

/// @todo unbenutzt???
// Actor* getActor oder andere OUTPUT Parameter
%typemap(out) SWIGTYPE*
"   $result = getInScriptObject((void *) $1, $1_descriptor );" 

%{
static VALUE getDirectorInScriptObject( Swig::Director * dir, void* ptr, swig_type_info *type )
{
    // Auf Director testen
    if ( dir ) 
        return dir->swig_get_self();

	return getInScriptObject( ptr, type );
}
%}

/// @todo unbenutzt???
// doWithActor( Actor* ) oder andere Director-Methoden Parameter
%typemap(directorin) SWIGTYPE*
"   $input = getDirectorInScriptObject(dynamic_cast< Swig::Director * >($1), (void *)$1, $1_descriptor);" 

%{
static VALUE getOutScriptObject( void* ptr, swig_type_info *type )
{
	VALUE val = SWIG_RubyInstanceFor( ptr );
	
	// Es gab das SkriptObjekt noch nicht
	if (NIL_P(val))
	{
		// Dynamic Cast ausfhren
		swig_type_info *ty = SWIG_TypeDynamicCast(type, &ptr);
		return SWIG_NewPointerObj( ptr, ty, 0);		
	}
	else	
		return val;
}
%}

// Animation* getActor oder andere OUTPUT Parameter f¸r DYNAMICs
%typemap(out) SWIGTYPE* DYNAMIC, SWIGTYPE& DYNAMIC
"   $result = getOutScriptObject((void *) $1, $1_descriptor );"

%{
static VALUE getDirectorOutScriptObject( Swig::Director * dir, void* ptr, swig_type_info *type )
{
	if (dir) 
        return dir->swig_get_self();        	

	return getOutScriptObject( ptr, type );
}
%}

// doWithAnimation( Animation* ) oder andere Director-Methoden Parameter fr DYNAMICs
%typemap(directorin) SWIGTYPE* DYNAMIC, SWIGTYPE& DYNAMIC
"   $input = getDirectorOutScriptObject(dynamic_cast< Swig::Director * >($1), (void *) $1, $1_descriptor );" 
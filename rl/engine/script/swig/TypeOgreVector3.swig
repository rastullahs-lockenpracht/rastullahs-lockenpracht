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

/* Typemaps fuer Vector3.
 * Ein Vector3 wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */

%{
// Helper method for checking the Vector3
static int checkRubyVector3( VALUE input ) 
{
   bool correct = 
       TYPE(input) == T_ARRAY   && 
       RARRAY_LEN(input) == 3  &&
       SWIG_FLOAT_P( RARRAY_PTR(input)[0] ) &&
       SWIG_FLOAT_P( RARRAY_PTR(input)[1] ) &&
       SWIG_FLOAT_P( RARRAY_PTR(input)[2] );

    return (correct ? 1 : 0);
}
%}

%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY) 
   Ogre::Vector3, const Ogre::Vector3, 
   Ogre::Vector3*, Ogre::Vector3&,
   const Ogre::Vector3*, const Ogre::Vector3& 
"$1 = checkRubyVector3($input);"
 
%{
// Helper method for filling the Vector3
static void convertRubyToVector3( VALUE input, Ogre::Vector3* v ) 
{
   int length = RARRAY_LEN(input);
   VALUE* it = RARRAY_PTR(input);
   if (length > 0) {
      v->x = NUM2DBL(*it);
      it++;
   }
   if (length > 1) {
      v->y = NUM2DBL(*it);
      it++;
   }
   if (length > 2) {
      v->z = NUM2DBL(*it);
      it++;
   }
}
%}

%typemap(in) Ogre::Vector3, const Ogre::Vector3
%{
   convertRubyToVector3( $input, & $1 );
%}

%typemap(in) Ogre::Vector3*, Ogre::Vector3&,
   const Ogre::Vector3*, const Ogre::Vector3&
%{
   $1 = new Ogre::Vector3(0.0, 0.0, 0.0);
   convertRubyToVector3(  $input, $1 );
%}

%{
// Helper method for creating the ruby Array
static VALUE convertVector3ToRuby( const Ogre::Vector3* v ) 
{
   VALUE array = rb_ary_new3(3,
	rb_float_new(v->x), 
	rb_float_new(v->y), 
	rb_float_new(v->z) );
	
   return array;
}
%}

%typemap(out) Ogre::Vector3, const Ogre::Vector3 
"   $result = convertVector3ToRuby( & $1 );"

%typemap(out) Ogre::Vector3*, const Ogre::Vector3*, const Ogre::Vector3&, Ogre::Vector3& 
"   $result = convertVector3ToRuby( $1 );" 


%typemap(freearg) Ogre::Vector3*, Ogre::Vector3&,
   const Ogre::Vector3*, const Ogre::Vector3&  
"   delete $1;"


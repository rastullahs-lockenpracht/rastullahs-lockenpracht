/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

/* Typemaps fuer ColourValue.
 * Ein ColourValue wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */

%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY) 
   Ogre::ColourValue, const Ogre::ColourValue, 
   Ogre::ColourValue*, Ogre::ColourValue&,
   const Ogre::ColourValue*, const Ogre::ColourValue&
{ 
   bool correct = 
       TYPE($input) == T_ARRAY   && 
       RARRAY($input)->len == 4  &&
       SWIG_FLOAT_P( RARRAY($input)->ptr[0] ) &&
       SWIG_FLOAT_P( RARRAY($input)->ptr[1] ) &&
       SWIG_FLOAT_P( RARRAY($input)->ptr[2] ) &&
       SWIG_FLOAT_P( RARRAY($input)->ptr[3] );

   $1 = correct ? 1 : 0;
}
 
%{
// Helper method for filling the ColourValue
static void convertRubyToColourValue( VALUE input, Ogre::ColourValue* cv ) 
{
   int length = RARRAY(input)->len;
   VALUE* it = RARRAY(input)->ptr;
   if (length > 0) {
      cv->r = NUM2DBL(*it);
      it++;
   }
   if (length > 1) {
      cv->g = NUM2DBL(*it);
      it++;
   }
   if (length > 2) {
      cv->b = NUM2DBL(*it);
      it++;
   }
   if (length > 3) {
      cv->a = NUM2DBL(*it);
   }
}
%}

%typemap(in) Ogre::ColourValue, const Ogre::ColourValue
{
   Ogre::ColourValue vec(0.0, 0.0, 0.0);
   convertRubyToColourValue( $input, & vec );
   $1 = vec;
}

%typemap(in) Ogre::ColourValue*, Ogre::ColourValue&,
   const Ogre::ColourValue*, const Ogre::ColourValue&
{
   Ogre::ColourValue* vec = new Ogre::ColourValue(0.0, 0.0, 0.0);
   convertRubyToColourValue(  $input, vec );
   $1 = vec;
}

%{
// Helper method for creating the ruby Array
static VALUE convertColourValueToRuby( const Ogre::ColourValue* cv ) 
{
   VALUE array = rb_ary_new3(4,
	rb_float_new(cv->r), 
	rb_float_new(cv->g), 
	rb_float_new(cv->b), 
	rb_float_new(cv->a) );
	
   return array;
}
%}

%typemap(out) Ogre::ColourValue, const Ogre::ColourValue {
   $result = convertColourValueToRuby( & $1 );
}

%typemap(out) Ogre::ColourValue*, const Ogre::ColourValue*, const Ogre::ColourValue&, Ogre::ColourValue& {
   $result = convertColourValueToRuby( $1 );
} 


%typemap(freearg) Ogre::ColourValue*, Ogre::ColourValue&,
   const Ogre::ColourValue*, const Ogre::ColourValue&  
"   delete $1;"
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

/* Typemaps fuer ColourValue.
 * Ein ColourValue wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */

%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY) Ogre::ColourValue, const Ogre::ColourValue, Ogre::ColourValue*, Ogre::ColourValue&,
   const Ogre::ColourValue*, const Ogre::ColourValue&
{
   $1 = TYPE($input) == T_ARRAY && RARRAY($input)->len == 4 ? 1 : 0;
}
 
%typemap(in) Ogre::ColourValue, const Ogre::ColourValue
{
   Ogre::ColourValue vec(0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      vec.r = RFLOAT(*it)->value;
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      vec.g = RFLOAT(*it)->value;
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      vec.b = RFLOAT(*it)->value;
   }
   if (length > 3) {
      Check_Type(*it, T_FLOAT);
      vec.a = RFLOAT(*it)->value;
   }
   $1 = vec;
}

%typemap(in) Ogre::ColourValue*, Ogre::ColourValue&,
   const Ogre::ColourValue*, const Ogre::ColourValue&
{
   Ogre::ColourValue* vec = new Ogre::ColourValue(0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      vec->r = NUM2DBL(*it);
      it++;
   }
   if (length > 1) {
      vec->g = NUM2DBL(*it);
      it++;
   }
   if (length > 2) {
      vec->b = NUM2DBL(*it);
      it++;
   }
   if (length > 3) {
      vec->a = NUM2DBL(*it);
   }
   $1 = vec;
}
%typemap(freearg) Ogre::ColourValue*, Ogre::ColourValue&,
   const Ogre::ColourValue*, const Ogre::ColourValue&  
"   delete $1;"

%typemap(out) Ogre::ColourValue, const Ogre::ColourValue {
   VALUE array = rb_ary_new();
   rb_ary_push(array, rb_float_new($1.r));
   rb_ary_push(array, rb_float_new($1.g));
   rb_ary_push(array, rb_float_new($1.b));
   rb_ary_push(array, rb_float_new($1.a));
   $result = array;
}

%typemap(out) Ogre::ColourValue*, const Ogre::ColourValue*, const Ogre::ColourValue&, Ogre::Vector& {
   VALUE array = rb_ary_new();
   rb_ary_push(array, rb_float_new($1->r));
   rb_ary_push(array, rb_float_new($1->g));
   rb_ary_push(array, rb_float_new($1->b));
   rb_ary_push(array, rb_float_new($1->a));
   $result = array;
} 

%typemap(freearg) Ogre::ColourValue*, Ogre::ColourValue&,
   const Ogre::ColourValue*, const Ogre::ColourValue&  
"   delete $1;"


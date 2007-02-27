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

/* Typemaps fuer Vector3.
 * Ein Vector3 wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */

%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY) Ogre::Vector3, const Ogre::Vector3, Ogre::Vector3*, Ogre::Vector3&,
   const Ogre::Vector3*, const Ogre::Vector3&
{
   $1 = TYPE($input) == T_ARRAY && RARRAY($input)->len == 3 ? 1 : 0;
}
 
%typemap(in) Ogre::Vector3, const Ogre::Vector3
{
   Ogre::Vector3 vec(0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      vec.x = NUM2DBL(*it);
      it++;
   }
   if (length > 1) {
      vec.y = NUM2DBL(*it);
      it++;
   }
   if (length > 2) {
      vec.z = NUM2DBL(*it);
   }
   $1 = vec;
}

%typemap(in) Ogre::Vector3*, Ogre::Vector3&,
   const Ogre::Vector3*, const Ogre::Vector3&
{
   Ogre::Vector3* vec = new Ogre::Vector3(0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      vec->x = NUM2DBL(*it);
      it++;
   }
   if (length > 1) {
      vec->y = NUM2DBL(*it);
      it++;
   }
   if (length > 2) {
      vec->z = NUM2DBL(*it);
   }
   $1 = vec;
}
%typemap(freearg) Ogre::Vector3*, Ogre::Vector3&,
   const Ogre::Vector3*, const Ogre::Vector3& {
   delete $1;
}

%typemap(out) Ogre::Vector3, const Ogre::Vector3 {
   VALUE rval = rb_ary_new();
   rb_ary_push(rval, rb_float_new($1.x));
   rb_ary_push(rval, rb_float_new($1.y));
   rb_ary_push(rval, rb_float_new($1.z));
   $result = rval;
}

%typemap(out) Ogre::Vector3*, const Ogre::Vector3*, const Ogre::Vector3&, Ogre::Vector& {
   VALUE rval = rb_ary_new();
   rb_ary_push(rval, rb_float_new($1->x));
   rb_ary_push(rval, rb_float_new($1->y));
   rb_ary_push(rval, rb_float_new($1->z));
   $result = rval;
} 



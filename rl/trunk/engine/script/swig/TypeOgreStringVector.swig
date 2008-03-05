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

%typemap(freearg) Ogre::StringVector &, const Ogre::StringVector & {
  delete $1;
}

%typemap(directorin) Ogre::StringVector &, const Ogre::StringVector & {
  VALUE arr = rb_ary_new2($1->size()); 
  StringVector::iterator i = $1->begin(), iend = $1->end();
  for ( ; i!=iend; i++ )
    rb_ary_push(arr, rb_str_new2(&(*i)));
  $result = arr;
}
%typemap(directorin) Ogre::StringVector, const Ogre::StringVector {
  VALUE arr = rb_ary_new2($1.size()); 
  StringVector::iterator i = $1.begin(), iend = $1.end();
  for ( ; i!=iend; i++ )
    rb_ary_push(arr, rb_str_new2(*i));
  $result = arr;
}

%typemap(directorout) Ogre::StringVector , const Ogre::StringVector {
  Check_Type($input, T_ARRAY);
  Ogre::StringVector vec;
  int len = RARRAY($input)->len;
  for (int i=0; i!=len; i++) {
    VALUE inst = rb_ary_entry($input, i);
    vec.push_back(Ogre::String(StringValuePtr(inst)));
  }
  $result = vec;
}

%typemap(directorout) Ogre::StringVector *, const Ogre::StringVector* , Ogre::StringVector &, const Ogre::StringVector& {
  Check_Type($input, T_ARRAY);
  Ogre::StringVector *vec = new Ogre::StringVector;
  int len = RARRAY($input)->len;
  for (int i=0; i!=len; i++) {
    VALUE inst = rb_ary_entry($input, i);
    vec->push_back(Ogre::String(StringValuePtr(inst)));
  }
  $result = vec;
}

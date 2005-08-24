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

#ifndef __RL_DYNAMICCAST_I__
#define __RL_DYNAMICCAST_I__

/* Wrapping Strings to ruby and back 
 for String, const String, String&, String*, const String*, const String&
*/
%typemap(typecheck) Ogre::String, const Ogre::String & = char *;

%typemap(in) Ogre::String, const Ogre::String {
    Check_Type($input, T_STRING);
    $1 = Ogre::String(StringValuePtr($input));
}
%typemap(out) Ogre::String, const Ogre::String {
     $result = rb_str_new2($1.c_str());
}
%typemap(in) Ogre::String*, Ogre::String&, const Ogre::String*, const Ogre::String& {
    Check_Type($input, T_STRING);
    $1 = new Ogre::String(StringValuePtr($input));
}
%typemap(out) Ogre::String*, Ogre::String&, const Ogre::String*, const Ogre::String& {
     $result = rb_str_new2($1->c_str());
}


/* Wrapping rl::CeGuiStrings to ruby and back 
 for rl::CeGuiString, const rl::CeGuiString, rl::CeGuiString&, rl::CeGuiString*, const rl::CeGuiString*, const rl::CeGuiString&
*/
%typemap(typecheck) rl::CeGuiString, const rl::CeGuiString & = char *;

%typemap(in) rl::CeGuiString, const rl::CeGuiString {
    Check_Type($input, T_STRING);
    $1 = rl::CeGuiString((CEGUI::utf8*)StringValuePtr($input));
}
%typemap(out) rl::CeGuiString, const rl::CeGuiString {
     $result = rb_str_new2($1.c_str());
}
%typemap(in) rl::CeGuiString*, rl::CeGuiString&, const rl::CeGuiString*, const rl::CeGuiString& {
    Check_Type($input, T_STRING);
    $1 = new rl::CeGuiString((CEGUI::utf8*)StringValuePtr($input));
}
%typemap(out) rl::CeGuiString*, rl::CeGuiString&,  const rl::CeGuiString*, const rl::CeGuiString& {
     $result = rb_str_new2($1->c_str());
}

%typemap(directorin) rl::CeGuiString, const rl::CeGuiString &, rl::CeGuiString & 
	"$input = rb_str_new2($1.c_str());"

%typemap(directorin) rl::CeGuiString *, const rl::CeGuiString * 
	"$input = rb_str_new2($1->c_str());"

%typemap(directorout) rl::CeGuiString, const rl::CeGuiString {
    if (TYPE($input) == T_STRING)
        $result = rl::CeGuiString((CEGUI::utf8*)StringValuePtr($input));
    else
        throw Swig::DirectorTypeMismatchException("string expected");
}

%typemap(directorout) const rl::CeGuiString &, rl::CeGuiString&  {
    if (TYPE($input) == T_STRING) {
		$result = new rl::CeGuiString();
        $result->assign(rl::CeGuiString((CEGUI::utf8*)StringValuePtr($input)));
    } else {
        throw Swig::DirectorTypeMismatchException("string expected");
    }
}

/* Radian / Degree all Ruby Values are interpreted as DEGREE! */
%typemap(typecheck) Ogre::Radian, const Ogre::Radian& = double;

%typemap(in) Ogre::Radian, const Ogre::Radian, Ogre::Radian&, const Ogre::Radian& {
    Check_Type($input, T_FLOAT);
    $1 = Ogre::Degree(RFLOAT($input)->value);
}
%typemap(in) Ogre::Radian*, const Ogre::Radian*, Ogre::Radian&, const Ogre::Radian& {
    Check_Type($input, T_FLOAT);
    $1 = new Ogre::Radian(Ogre::Degree(RFLOAT($input)->value));
}
%typemap(out) Ogre::Radian, const Ogre::Radian {
     $result = rb_float_new($1.valueDegrees());
}
%typemap(out) Ogre::Radian&, const Ogre::Radian& {
     $result = rb_float_new($1->valueDegrees());
}


/* Wrapping Real to ruby and back 
 for Real, const Real, Real&, Real*, const Real*, const Real&
*/
%typemap(typecheck) Ogre::Real, const Ogre::Real& = double;
    
%typemap(in) Ogre::Real, const Ogre::Real {
    Check_Type($input, T_FLOAT);
    $1 = RFLOAT($input)->value;
}
%typemap(out) Ogre::Real, const Ogre::Real {
     $result = rb_float_new($1);
}
%typemap(in) Ogre::Real*, Ogre::Real&, const Ogre::Real*, const Ogre::Real& {
    Check_Type($input, T_FLOAT);
    $1 = RFLOAT($input)->value;
}
%typemap(out) Ogre::Real*, Ogre::Real&,  const Ogre::Real*, const Ogre::Real& {
	$result = rb_float_new(*$1);
}

/* IN Typemaps fuer Tripel<int>.
 * Ein Tripel wird einfach auf ein dreielementiges Array abgebildet.
 * 
 * Eine Laengenueberpruefung findet nicht statt.
 * Nil wird zu 0. Indizes >= 3 werden ignoriert.
 * TODO eben das aendern
 */
%typemap(in) Tripel<int>
{
   Check_Type($input, T_ARRAY);
   Tripel<int> tripel(0, 0, 0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      tripel.first = FIX2INT(*it);
      it++;
   }
   if (length > 1) {
      tripel.second = FIX2INT(*it);
      it++;
   }
   if (length > 2) {
      tripel.third = FIX2INT(*it);
   }
   $1 = tripel;
}

%typemap(in) Tripel<int>*, Tripel<int>&,
   const Tripel<int>*, const Tripel<int>&
{
   Check_Type($input, T_ARRAY);
   Tripel<int>* tripel = new Tripel<int>(0, 0, 0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      tripel->first = FIX2INT(*it);
      it++;
   }
   if (length > 1) {
      tripel->second = FIX2INT(*it);
      it++;
   }
   if (length > 2) {
      tripel->third = FIX2INT(*it);
   }
   $1 = tripel;
}

/* OUT Typemaps fuer Tripel<int>.
 * Ein Tripel wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */
%typemap(out) Tripel<int> {
   VALUE array = rb_ary_new();
   rb_ary_push(array, INT2FIX($1.first));
   rb_ary_push(array, INT2FIX($1.second));
   rb_ary_push(array, INT2FIX($1.third));
   $result = array;
}

/* IN Typemaps fuer Vector3.
 * Ein Tripel wird einfach auf ein dreielementiges Array abgebildet.
 * 
 * Eine Laengenueberpruefung findet nicht statt.
 * Nil wird zu 0. Indizes >= 3 werden ignoriert.
 * TODO eben das aendern
 */

%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY) Ogre::Vector3, const Ogre::Vector3& {
  $1 = TYPE($input) == T_ARRAY && RARRAY($input)->len == 3 ? 1 : 0;
}
 
%typemap(in) Ogre::Vector3
{
   Check_Type($input, T_ARRAY);
   Ogre::Vector3 vec(0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      vec.x = RFLOAT(*it)->value;
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      vec.y = RFLOAT(*it)->value;
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      vec.z = RFLOAT(*it)->value;
   }
   $1 = vec;
}

%typemap(in) Ogre::Vector3*, Ogre::Vector3&,
   const Ogre::Vector3*, const Ogre::Vector3&
{
   Check_Type($input, T_ARRAY);
   Ogre::Vector3* vec = new Ogre::Vector3(0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      vec->x = RFLOAT(*it)->value;
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      vec->y = RFLOAT(*it)->value;
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      vec->z = RFLOAT(*it)->value;
   }
   $1 = vec;
}

%typemap(in) Ogre::Quaternion
{
   Check_Type($input, T_ARRAY);
   Ogre::Quaternion quat(0.0, 0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      quat.w = RFLOAT(*it)->value;
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      quat.x = RFLOAT(*it)->value;
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      quat.y = RFLOAT(*it)->value;
      it++;
   }
   if (length > 3) {
      Check_Type(*it, T_FLOAT);
      quat.z = RFLOAT(*it)->value;
   }
   $1 = quat;
}


%typecheck(SWIG_TYPECHECK_DOUBLE_ARRAY) Ogre::Quaternion, const Ogre::Quaternion& {
  $1 = TYPE($input) == T_ARRAY && RARRAY($input)->len == 4 ? 1 : 0;
}

%typemap(in) Ogre::Quaternion*, Ogre::Quaternion&,
   const Ogre::Quaternion*, const Ogre::Quaternion&
{
   Check_Type($input, T_ARRAY);
   Ogre::Quaternion* quat = new Ogre::Quaternion(0.0, 0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      quat->w = RFLOAT(*it)->value;
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      quat->x = RFLOAT(*it)->value;
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      quat->y = RFLOAT(*it)->value;
      it++;
   }
   if (length > 3) {
      Check_Type(*it, T_FLOAT);
      quat->z = RFLOAT(*it)->value;
   }
   $1 = quat;
}

/* OUT Typemaps fuer Tripel<int>.
 * Ein Tripel wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */
%typemap(out) Ogre::Vector3, const Ogre::Vector3 {
   VALUE array = rb_ary_new();
   rb_ary_push(array, rb_float_new($1.x));
   rb_ary_push(array, rb_float_new($1.y));
   rb_ary_push(array, rb_float_new($1.z));
   $result = array;
}

%typemap(out) Ogre::Vector3*, const Ogre::Vector3*, const Ogre::Vector3&, Ogre::Vector& {
   VALUE array = rb_ary_new();
   rb_ary_push(array, rb_float_new($1->x));
   rb_ary_push(array, rb_float_new($1->y));
   rb_ary_push(array, rb_float_new($1->z));
   $result = array;
} 

%typemap(out) Ogre::Quaternion, const Ogre::Quaternion {
   VALUE array = rb_ary_new();
   rb_ary_push(array, rb_float_new($1.w));
   rb_ary_push(array, rb_float_new($1.x));
   rb_ary_push(array, rb_float_new($1.y));
   rb_ary_push(array, rb_float_new($1.z));
   $result = array;
}

%typemap(out) Ogre::Quaternion*, const Ogre::Quaternion*, const Ogre::Quaternion&, Ogre::Quaternion& {
   VALUE array = rb_ary_new();
   rb_ary_push(array, rb_float_new($1->w));
   rb_ary_push(array, rb_float_new($1->x));
   rb_ary_push(array, rb_float_new($1->y));
   rb_ary_push(array, rb_float_new($1->z));
   $result = array;
} 

%typemap(ruby, freearg) rl::CeGuiStringVector &, const rl::CeGuiStringVector & {
  delete $1;
}

%typemap(ruby, directorin) rl::CeGuiStringVector &, const rl::CeGuiStringVector & {
  VALUE arr = rb_ary_new2($1->size()); 
  StringVector::iterator i = $1->begin(), iend = $1->end();
  for ( ; i!=iend; i++ )
    rb_ary_push(arr, rb_str_new2(&(*i)));
  $result = arr;
}
%typemap(ruby, directorin) rl::CeGuiStringVector, const rl::CeGuiStringVector {
  VALUE arr = rb_ary_new2($1.size()); 
  StringVector::iterator i = $1.begin(), iend = $1.end();
  for ( ; i!=iend; i++ )
    rb_ary_push(arr, rb_str_new2(*i));
  $result = arr;
}

%typemap(ruby, directorout) rl::CeGuiStringVector , const rl::CeGuiStringVector {
  Check_Type($input, T_ARRAY);
  rl::CeGuiStringVector vec;
  int len = RARRAY($input)->len;
  for (int i=0; i!=len; i++) {
    VALUE inst = rb_ary_entry($input, i);
    vec.push_back(rl::CeGuiString(StringValuePtr(inst)));
  }
  $result = vec;
}

%typemap(ruby, directorout) rl::CeGuiStringVector &, const rl::CeGuiStringVector& {
  Check_Type($input, T_ARRAY);
  rl::CeGuiStringVector *vec = new rl::CeGuiStringVector;
  int len = RARRAY($input)->len;
  for (int i=0; i!=len; i++) {
    VALUE inst = rb_ary_entry($input, i);
    vec->push_back(rl::CeGuiString(StringValuePtr(inst)));
  }
  $result = vec;
}

#endif
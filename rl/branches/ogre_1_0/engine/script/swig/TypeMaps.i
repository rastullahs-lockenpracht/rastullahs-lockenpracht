/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __RL_DYNAMICCAST_I__
#define __RL_DYNAMICCAST_I__

/* Wrapping Strings to ruby and back 
 for String, const String, String&, String*, const String*, const String&
*/
%typemap(in) String, const String {
    Check_Type($input, T_STRING);
    $1 = String(StringValuePtr($input));
}
%typemap(out) String, const String {
     $result = rb_str_new2($1.c_str());
}
%typemap(in) String*, String&, const String*, const String& {
    Check_Type($input, T_STRING);
    $1 = new String(StringValuePtr($input));
}
%typemap(out) String*, String&,  const String*, const String& {
     $result = rb_str_new2($1->c_str());
}

/* Wrapping rl::CeGuiStrings to ruby and back 
 for rl::CeGuiString, const rl::CeGuiString, rl::CeGuiString&, rl::CeGuiString*, const rl::CeGuiString*, const rl::CeGuiString&
*/
%typemap(in) rl::CeGuiString, const rl::CeGuiString {
    Check_Type($input, T_STRING);
    $1 = rl::CeGuiString(StringValuePtr($input));
}
%typemap(out) rl::CeGuiString, const rl::CeGuiString {
     $result = rb_str_new2($1.c_str());
}
%typemap(in) rl::CeGuiString*, rl::CeGuiString&, const rl::CeGuiString*, const rl::CeGuiString& {
    Check_Type($input, T_STRING);
    $1 = new rl::CeGuiString(StringValuePtr($input));
}
%typemap(out) rl::CeGuiString*, rl::CeGuiString&,  const rl::CeGuiString*, const rl::CeGuiString& {
     $result = rb_str_new2($1->c_str());
}

%typemap(directorin) rl::CeGuiString, const rl::CeGuiString &, rl::CeGuiString & 
	"$input = rb_str_new2($1.c_str());"

%typemap(directorin) rl::CeGuiString *, const rl::CeGuiString * 
	"$input = rb_str_new2($1->c_str());"

%typemap(directorout) rl::CeGuiString {
    if (TYPE($input) == T_STRING)
        $result = rl::CeGuiString(StringValuePtr($input));
    else
        throw Swig::DirectorTypeMismatchException("string expected");
}

%typemap(directorout) const rl::CeGuiString & {
    if (TYPE($input) == T_STRING) {
		$result = new rl::CeGuiString();
        $result->assign(rl::CeGuiString(StringValuePtr($input)));
    } else {
        throw Swig::DirectorTypeMismatchException("string expected");
    }
}

/* Radian / Degree all Ruby Values are interpreted as DEGREE! */
%typemap(in) Radian, const Radian {
    Check_Type($input, T_FLOAT);
    $1 = Degree(RFLOAT($input)->value);
}
%typemap(out) Radian, const Radian {
     $result = rb_float_new($1.valueDegrees());
}

    
/* Wrapping Real to ruby and back 
 for Real, const Real, Real&, Real*, const Real*, const Real&
*/
%typemap(in) Real, const Real {
    Check_Type($input, T_FLOAT);
    $1 = RFLOAT($input)->value;
}
%typemap(out) Real, const Real {
     $result = rb_float_new($1);
}
%typemap(in) Real*, Real&, const Real*, const Real& {
    Check_Type($input, T_FLOAT);
    $1 = RFLOAT($input)->value;
}
%typemap(out) Real*, Real&,  const Real*, const Real& {
	$result = rb_float_new(*$1);
}

/* Wrapping float to ruby and back 
 for float, const float, float&, float*, const float*, const float&
*/
%typemap(in) float, const float {
    Check_Type($input, T_FLOAT);
    $1 = RFLOAT($input)->value;
}

//%typemap(out) float, const float {
//     $result = $1;
//}

%typemap(in) float*, float&, const float*, const float& {
    Check_Type($input, T_FLOAT);
    $1 = RFLOAT($input)->value;
}

//%typemap(out) float*, float&,  const float*, const float& {
//	$result = $1;
//}

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
%typemap(in) Vector3
{
   Check_Type($input, T_ARRAY);
   Vector3 vector(0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      vector.x = RFLOAT(*it)->value;
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      vector.y = RFLOAT(*it)->value;
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      vector.z = RFLOAT(*it)->value;
   }
   $1 = vector;
}

%typemap(in) Vector3*, Vector3&,
   const Vector3*, const Vector3&
{
   Check_Type($input, T_ARRAY);
   Vector3* vector = new Vector3(0.0, 0.0, 0.0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      vector->x = RFLOAT(*it)->value;
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      vector->y = RFLOAT(*it)->value;
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      vector->z = RFLOAT(*it)->value;
   }
   $1 = vector;
}

/* OUT Typemaps fuer Tripel<int>.
 * Ein Tripel wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */
%typemap(out) Vector3 {
   VALUE array = rb_ary_new();
   rb_ary_push(array, rb_float_new($1.x));
   rb_ary_push(array, rb_float_new($1.y));
   rb_ary_push(array, rb_float_new($1.z));
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
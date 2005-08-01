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

/* Wrapping rl::CeGuiStrings to ruby and back 
 for rl::CeGuiString, const rl::CeGuiString, rl::CeGuiString&, rl::CeGuiString*, const rl::CeGuiString*, const rl::CeGuiString&
*/
%typemap(in) rl::CeGuiString, const rl::CeGuiString {
    Check_Type($input, T_STRING);
    $1 = rl::CeGuiString(PyString_AsString($input));
}
%typemap(out) rl::CeGuiString, const rl::CeGuiString {
     $result = PyString_FromString($1.c_str());
}
%typemap(in) rl::CeGuiString*, rl::CeGuiString&, const rl::CeGuiString*, const rl::CeGuiString& {
    Check_Type($input, T_STRING);
    $1 = new rl::CeGuiString(PyString_AsString($input));
}
%typemap(out) rl::CeGuiString*, rl::CeGuiString&,  const rl::CeGuiString*, const rl::CeGuiString& {
     $result = PyString_FromString($1->c_str());
}

%typemap(directorin) rl::CeGuiString, const rl::CeGuiString &, rl::CeGuiString & 
	"$input = PyString_FromString($1.c_str());"

%typemap(directorin) rl::CeGuiString *, const rl::CeGuiString * 
	"$input = PyString_FromString($1->c_str());"

%typemap(directorout) rl::CeGuiString {
    if (TYPE($input) == T_STRING)
        $result = rl::CeGuiString(PyString_AsString($input));
    else
        throw Swig::DirectorTypeMismatchException("string expected");
}

%typemap(directorout) const rl::CeGuiString & {
    if (TYPE($input) == T_STRING) {
		$result = new rl::CeGuiString();
        $result->assign(rl::CeGuiString(PyString_AsString($input)));
    } else {
        throw Swig::DirectorTypeMismatchException("string expected");
    }
}

/* Radian / Degree all Ruby Values are interpreted as DEGREE! */
%typemap(in) Radian, const Radian, Radian&, const Radian& {
    Check_Type($input, T_FLOAT);
    $1 = Degree(PyFloat_AsDouble($input));
}
%typemap(in) Radian*, const Radian*, Radian&, const Radian& {
    Check_Type($input, T_FLOAT);
    $1 = new Radian(Degree(PyFloat_AsDouble($input)));
}
%typemap(out) Radian, const Radian {
     $result = PyFloat_FromDouble($1.valueDegrees());
}
%typemap(out) Radian&, const Radian& {
     $result = PyFloat_FromDouble($1->valueDegrees());
}

    
/* Wrapping Real to ruby and back 
 for Real, const Real, Real&, Real*, const Real*, const Real&
*/
%typemap(in) Real, const Real {
    Check_Type($input, T_FLOAT);
    $1 = PyFloat_AsDouble($input);
}
%typemap(out) Real, const Real {
     $result = PyFloat_FromDouble($1);
}
%typemap(in) Real*, Real&, const Real*, const Real& {
    Check_Type($input, T_FLOAT);
    $1 = PyFloat_AsDouble($input);
}
%typemap(out) Real*, Real&,  const Real*, const Real& {
	$result = PyFloat_FromDouble(*$1);
}

/* Wrapping float to ruby and back 
 for float, const float, float&, float*, const float*, const float&
*/
%typemap(in) float, const float {
    Check_Type($input, T_FLOAT);
    $1 = PyFloat_AsDouble($input);
}

//%typemap(out) float, const float {
//     $result = $1;
//}

%typemap(in) float*, float&, const float*, const float& {
    Check_Type($input, T_FLOAT);
    $1 = PyFloat_AsDouble($input);
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
   int length = PyTuple_Size($input);
   if (length > 0) {
      tripel.first = PyTuple_GetItem($input, 0);
   }
   if (length > 1) {
      tripel.second = PyTuple_GetItem($input, 1);
   }
   if (length > 2) {
      tripel.third = PyTuple_GetItem($input, 2);
   }
   $1 = tripel;
}

%typemap(in) Tripel<int>*, Tripel<int>&,
   const Tripel<int>*, const Tripel<int>&
{
   Check_Type($input, T_ARRAY);
   Tripel<int>* tripel = new Tripel<int>(0, 0, 0);
   int length = PyTuple_Size($input);
   if (length > 0) {
      tripel.first = PyInt_AsLong(PyTuple_GetItem($input, 0));
   }
   if (length > 1) {
      tripel.second = PyInt_AsLong(PyTuple_GetItem($input, 1));
   }
   if (length > 2) {
      tripel.third = PyInt_AsLong(PyTuple_GetItem($input, 2));
   }
   $1 = tripel;
}

/* OUT Typemaps fuer Tripel<int>.
 * Ein Tripel wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */
%typemap(out) Tripel<int> {
   PyObject* array = PyTuple_New(3);
   PyTuple_SetItem(array, 0, PyInt_FromLong($1.first));
   PyTuple_SetItem(array, 1, PyInt_FromLong($1.second));
   PyTuple_SetItem(array, 2, PyInt_FromLong($1.third));
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
   int length = PyTuple_Size($input);
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      vector.x = PyFloat_AsDouble(PyTuple_GetItem($input, 0));
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      vector.y = PyFloat_AsDouble(PyTuple_GetItem($input, 1));
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      vector.z = PyFloat_AsDouble(PyTuple_GetItem($input, 2));
   }
   $1 = vector;
}

%typemap(in) Vector3*, Vector3&,
   const Vector3*, const Vector3&
{
   Check_Type($input, T_ARRAY);
   Vector3* vector = new Vector3(0.0, 0.0, 0.0);
   int length = PyTuple_Size($input);
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      vector->x = PyFloat_AsDouble(PyTuple_GetItem($input, 0));
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      vector->y = PyFloat_AsDouble(PyTuple_GetItem($input, 1));
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      vector->z = PyFloat_AsDouble(PyTuple_GetItem($input, 2));
   }
   $1 = vector;
}

%typemap(in) Quaternion
{
   Check_Type($input, T_ARRAY);
   Quaternion quat(0.0, 0.0, 0.0, 0.0);
   int length = PyTuple_Size($input);
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      quat.w = PyFloat_AsDouble(PyTuple_GetItem($input, 0));
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      quat.x = PyFloat_AsDouble(PyTuple_GetItem($input, 1));
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      quat.y = PyFloat_AsDouble(PyTuple_GetItem($input, 2));
   }
   if (length > 3) {
      Check_Type(*it, T_FLOAT);
      quat.z = PyFloat_AsDouble(PyTuple_GetItem($input, 3));
   }
   $1 = vector;
}

%typemap(in) Quaternion*, Quaternion&,
   const Quaternion*, const Quaternion&
{
   Check_Type($input, T_ARRAY);
   Quaternion* quat = new Quaternion(0.0, 0.0, 0.0, 0.0);
   int length = PyTuple_Size($input);
   if (length > 0) {
      Check_Type(*it, T_FLOAT);
      quat->w = PyFloat_AsDouble(PyTuple_GetItem($input, 0));
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      quat->x = PyFloat_AsDouble(PyTuple_GetItem($input, 1));
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      quat->y = PyFloat_AsDouble(PyTuple_GetItem($input, 2));
   }
   if (length > 3) {
      Check_Type(*it, T_FLOAT);
      quat->z = PyFloat_AsDouble(PyTuple_GetItem($input, 3));
   }
   $1 = vector;
}

/* OUT Typemaps fuer Tripel<int>.
 * Ein Tripel wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */
%typemap(out) Vector3, const Vector3 {
   PyObject* array = PyTuple_New(3);
   PyTuple_SetItem(array, 0, PyFloat_FromDouble($1.x));
   PyTuple_SetItem(array, 1, PyFloat_FromDouble($1.y));
   PyTuple_SetItem(array, 2, PyFloat_FromDouble($1.z));
   $result = array;
}

%typemap(out) Vector3*, const Vector3*, const Vector3&, Vector& {
   PyObject* array = PyTuple_New(3);
   PyTuple_SetItem(array, 0, PyFloat_FromDouble($1->x));
   PyTuple_SetItem(array, 1, PyFloat_FromDouble($1->y));
   PyTuple_SetItem(array, 2, PyFloat_FromDouble($1->z));
   $result = array;
} 

%typemap(out) Quaternion, const Quaternion {
   PyObject* array = PyTuple_New(3);
   PyTuple_SetItem(array, 0, PyFloat_FromDouble($1.w));
   PyTuple_SetItem(array, 1, PyFloat_FromDouble($1.x));
   PyTuple_SetItem(array, 2, PyFloat_FromDouble($1.y));
   PyTuple_SetItem(array, 3, PyFloat_FromDouble($1.z));
   $result = array;
}

%typemap(out) Quaternion*, const Quaternion*, const Quaternion&, Quaternion& {
   PyObject* array = PyTuple_New(3);
   PyTuple_SetItem(array, 0, PyFloat_FromDouble($1->w));
   PyTuple_SetItem(array, 1, PyFloat_FromDouble($1->x));
   PyTuple_SetItem(array, 2, PyFloat_FromDouble($1->y));
   PyTuple_SetItem(array, 3, PyFloat_FromDouble($1->z));
   $result = array;
} 

%typemap(ruby, freearg) rl::CeGuiStringVector &, const rl::CeGuiStringVector & {
  delete $1;
}

%typemap(ruby, directorin) rl::CeGuiStringVector &, const rl::CeGuiStringVector & {
  VALUE arr = rb_ary_new2($1->size()); 
  StringVector::iterator i = $1->begin(), iend = $1->end();
  for ( ; i!=iend; i++ )
    rb_ary_push(arr, PyString_FromString(&(*i)));
  $result = arr;
}
%typemap(ruby, directorin) rl::CeGuiStringVector, const rl::CeGuiStringVector {
  VALUE arr = rb_ary_new2($1.size()); 
  StringVector::iterator i = $1.begin(), iend = $1.end();
  for ( ; i!=iend; i++ )
    rb_ary_push(arr, PyString_FromString(*i));
  $result = arr;
}

%typemap(ruby, directorout) rl::CeGuiStringVector , const rl::CeGuiStringVector {
  Check_Type($input, T_ARRAY);
  rl::CeGuiStringVector vec;
  int len = PyTuple_Size($input);
  for (int i=0; i!=len; i++) {
    VALUE inst = rb_ary_entry($input, i);
    vec.push_back(rl::CeGuiString(PyString_AsString(inst)));
  }
  $result = vec;
}

%typemap(ruby, directorout) rl::CeGuiStringVector &, const rl::CeGuiStringVector& {
  Check_Type($input, T_ARRAY);
  rl::CeGuiStringVector *vec = new rl::CeGuiStringVector;
  int len = PyTuple_Size($input);
  for (int i=0; i!=len; i++) {
    VALUE inst = rb_ary_entry($input, i);
    vec->push_back(rl::CeGuiString(PyString_AsString(inst)));
  }
  $result = vec;
}

#endif
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
%typemap(varout) Ogre::String, const Ogre::String {
     $result = rb_str_new2($1.c_str());
}


/* Wrapping rl::CeGuiStrings to ruby and back 
 for CeGuiString
*/
%typemap(typecheck) rl::CeGuiString, const rl::CeGuiString, rl::CeGuiString*, rl::CeGuiString&, const rl::CeGuiString*, const rl::CeGuiString& = char *;

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
%typemap(varout) rl::CeGuiString, const rl::CeGuiString {
     $result = rb_str_new2($1.c_str());
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
    if (TYPE($input) == T_FLOAT) {
		$1 = Ogre::Degree(RFLOAT($input)->value);
    } else if (TYPE($input) == T_FIXNUM) {
		$1 = Ogre::Degree(FIX2INT($input));
    } else {
        throw Swig::DirectorTypeMismatchException("float or fixnum expected");
    }
}
%typemap(in) Ogre::Radian*, const Ogre::Radian*, Ogre::Radian&, const Ogre::Radian& {
    if (TYPE($input) == T_FLOAT) {
		$1 = new Ogre::Radian(Ogre::Degree(RFLOAT($input)->value));
    } else if (TYPE($input) == T_FIXNUM) {
		$1 = new Ogre::Radian(Ogre::Degree(FIX2INT($input)));
    } else {
        throw Swig::DirectorTypeMismatchException("float or fixnum expected");
    }
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
      Check_Type(*it, T_FLOAT);
      vec->r = RFLOAT(*it)->value;
      it++;
   }
   if (length > 1) {
      Check_Type(*it, T_FLOAT);
      vec->g = RFLOAT(*it)->value;
      it++;
   }
   if (length > 2) {
      Check_Type(*it, T_FLOAT);
      vec->b = RFLOAT(*it)->value;
      it++;
   }
   if (length > 3) {
      Check_Type(*it, T_FLOAT);
      vec->a = RFLOAT(*it)->value;
   }
   $1 = vec;
}

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


/* Typemaps fuer Quaternion.
 * Ein Quaternion wird auf ein Array aus einem 3-elementigen Array 
 * für die Rotationsachse und einem Wert für den Winkel abgebildet
 * 
 */

%typemap(typecheck) Ogre::Quaternion, const Ogre::Quaternion& {
		if (TYPE($input) == T_ARRAY)
		{
			int length = RARRAY($input)->len;
			if (length == 2)
			{
				VALUE entry0 = rb_ary_entry($input, 0);
				VALUE entry1 = rb_ary_entry($input, 1);
				if ((TYPE(entry1) == T_FLOAT || TYPE(entry1) == T_FIXNUM)
					&& TYPE(entry0) == T_ARRAY 
					&& RARRAY(entry0)->len == 3)
				{
					$1 = 1;
				}
				else
				{
					$1 = 0;
				}
			}
			else if (length == 3 || length == 4)
			{
				$1 = 1;
				
				for (int i = 0; i < length; i++)
				{
					if (TYPE(rb_ary_entry($input, i)) != T_FLOAT 
					    && TYPE(rb_ary_entry($input, i)) != T_FIXNUM)
					{
						$1 = 0;
						break;
					}
				}
			}
			else
			{
				$1 = 0;
			}
		}
		else
		{
			$1 = 0;
		}
}

%typemap(in) Ogre::Quaternion, const Ogre::Quaternion
{
   Ogre::Quaternion quat;
   
   if (RARRAY($input)->len == 2)
   {
	   VALUE axisVal = rb_ary_entry($input, 0);
	   Ogre::Vector3 axis(
				NUM2DBL(rb_ary_entry(axisVal, 0)), 
				NUM2DBL(rb_ary_entry(axisVal, 1)),
				NUM2DBL(rb_ary_entry(axisVal, 2)));

	   double angle = NUM2DBL(rb_ary_entry($input, 1));
	      
	   quat.FromAngleAxis(Ogre::Degree(angle), axis);
   }
   else if (RARRAY($input)->len == 3)
   {
	   Quaternion rotX, rotY, rotZ;
        rotX.FromAngleAxis(
			Ogre::Degree(NUM2DBL(rb_ary_entry(argv[1], 0))), 
			Ogre::Vector3::UNIT_X);
        rotY.FromAngleAxis(
			Ogre::Degree(NUM2DBL(rb_ary_entry(argv[1], 1))), 
			Ogre::Vector3::UNIT_Y);
        rotZ.FromAngleAxis(
			Ogre::Degree(NUM2DBL(rb_ary_entry(argv[1], 2))), 
			Ogre::Vector3::UNIT_Z);
	   
	   quat = rotX * rotY * rotZ;
	   quat.normalise();
   }
   else if (RARRAY($input)->len == 4)
   {
	   quat.w = NUM2DBL(rb_ary_entry($input, 0));
	   quat.x = NUM2DBL(rb_ary_entry($input, 1));
	   quat.y = NUM2DBL(rb_ary_entry($input, 2));
	   quat.z = NUM2DBL(rb_ary_entry($input, 3));
   }
   $1 = quat;
}

%typemap(in) Ogre::Quaternion*, Ogre::Quaternion&,
   const Ogre::Quaternion*, const Ogre::Quaternion&
{
   Ogre::Quaternion* quat = new Ogre::Quaternion();
   if (RARRAY($input)->len == 2)
   {
	   VALUE axisVal = rb_ary_entry($input, 0);
	   Ogre::Vector3 axis(
				NUM2DBL(rb_ary_entry(axisVal, 0)), 
				NUM2DBL(rb_ary_entry(axisVal, 1)),
				NUM2DBL(rb_ary_entry(axisVal, 2)));

	   double angle = NUM2DBL(rb_ary_entry($input, 1));
	      
	   quat->FromAngleAxis(Ogre::Degree(angle), axis);
   }
   else if (RARRAY($input)->len == 3)
   {
	   Quaternion rotX, rotY, rotZ;
        rotX.FromAngleAxis(
			Ogre::Degree(NUM2DBL(rb_ary_entry(argv[1], 0))), 
			Ogre::Vector3::UNIT_X);
        rotY.FromAngleAxis(
			Ogre::Degree(NUM2DBL(rb_ary_entry(argv[1], 1))), 
			Ogre::Vector3::UNIT_Y);
        rotZ.FromAngleAxis(
			Ogre::Degree(NUM2DBL(rb_ary_entry(argv[1], 2))), 
			Ogre::Vector3::UNIT_Z);
	   
	   *quat = rotX * rotY * rotZ;
	   quat->normalise();
   }
   else if (RARRAY($input)->len == 4)
   {
	   quat->w = NUM2DBL(rb_ary_entry($input, 0));
	   quat->x = NUM2DBL(rb_ary_entry($input, 1));
	   quat->y = NUM2DBL(rb_ary_entry($input, 2));
	   quat->z = NUM2DBL(rb_ary_entry($input, 3));
   }   
   $1 = quat;
}


%typemap(out) Ogre::Quaternion, const Ogre::Quaternion {
   Degree angle;
   Vector3 axis;
   
   $1.ToAngleAxis(angle, axis);
   
   VALUE arrAxis = rb_ary_new();
   rb_ary_push(arrAxis, rb_float_new(axis.x));
   rb_ary_push(arrAxis, rb_float_new(axis.y));
   rb_ary_push(arrAxis, rb_float_new(axis.z));
   VALUE rval = rb_ary_new();
   rb_ary_push(rval, rb_float_new(arrAxis));
   rb_ary_push(rval, rb_float_new(angle.valueDegrees()));
   
   $result = rval;
}

%typemap(out) Ogre::Quaternion*, const Ogre::Quaternion*, const Ogre::Quaternion&, Ogre::Quaternion& {
   Degree angle;
   Vector3 axis;
   
   $1->ToAngleAxis(angle, axis);
   
   VALUE arrAxis = rb_ary_new();
   rb_ary_push(arrAxis, rb_float_new(axis.x));
   rb_ary_push(arrAxis, rb_float_new(axis.y));
   rb_ary_push(arrAxis, rb_float_new(axis.z));
   VALUE rval = rb_ary_new();
   rb_ary_push(rval, rb_float_new(arrAxis));
   rb_ary_push(rval, rb_float_new(angle.valueDegrees()));
   
   $result = rval;
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
#ifndef __RL_DYNAMICCAST_I__
#define __RL_DYNAMICCAST_I__

/* Wrapping Strings to ruby and back 
 for String, const String, String&, String*, const String*, const String&
*/
%typemap(in) String, const String {
    Check_Type($input, T_STRING);
    $1 = String(STR2CSTR($input));
}
%typemap(out) String, const String {
     $result = rb_str_new2($1.c_str());
}
%typemap(in) String*, String&, const String*, const String& {
    Check_Type($input, T_STRING);
    $1 = new String(STR2CSTR($input));
}
%typemap(out) String*, String&,  const String*, const String& {
     $result = rb_str_new2($1->c_str());
}

/* Wrapping CeGuiStrings to ruby and back 
 for CeGuiString, const CeGuiString, CeGuiString&, CeGuiString*, const CeGuiString*, const CeGuiString&
*/
%typemap(in) CeGuiString, const CeGuiString {
    Check_Type($input, T_STRING);
    $1 = CeGuiString(STR2CSTR($input));
}
%typemap(out) CeGuiString, const CeGuiString {
     $result = rb_str_new2($1.c_str());
}
%typemap(in) CeGuiString*, CeGuiString&, const CeGuiString*, const CeGuiString& {
    Check_Type($input, T_STRING);
    $1 = new CeGuiString(STR2CSTR($input));
}
%typemap(out) CeGuiString*, CeGuiString&,  const CeGuiString*, const CeGuiString& {
     $result = rb_str_new2($1->c_str());
}

%typemap(directorin) CeGuiString, const CeGuiString &, CeGuiString & "$1_name.c_str();";

%typemap(directorin) CeGuiString *, const CeGuiString * "$1_name->c_str();";

%typemap(directorout) CeGuiString {
    if (TYPE($input) == T_STRING)
        $result = CeGuiString(STR2CSTR($input));
    else
        throw Swig::DirectorTypeMismatchException("string expected");
}

%typemap(directorout) const CeGuiString & {
    if (TYPE($input) == T_STRING) {
		$result = new CeGuiString();
        $result->assign(CeGuiString(STR2CSTR($input)));
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

#endif
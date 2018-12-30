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

/* Typemaps fuer Quaternion.
 * Ein Quaternion wird auf 
 *  - Ein Array aus einem 3-elementigen Array 
 *    fuer die Rotationsachse und einem Wert fuer den Winkel abgebildet
 *  - 
 * 
 */

%{
// Helper method for checking the Quaternion
static int checkRubyQuaternion( VALUE input ) 
{
	bool correct = false;

	if (TYPE(input) == T_ARRAY)
	{
                int length = RARRAY_LEN(input);

		if (length == 2)
		{
			VALUE arr = rb_ary_entry(input, 0);
			VALUE degree = rb_ary_entry(input, 1);

			if( SWIG_FLOAT_P(degree) &&
				TYPE(arr) == T_ARRAY && 
                                RARRAY_LEN(arr) == 3 &&
                                SWIG_FLOAT_P( RARRAY_PTR(arr)[0] ) &&
                                SWIG_FLOAT_P( RARRAY_PTR(arr)[1] ) &&
                                SWIG_FLOAT_P( RARRAY_PTR(arr)[2] ) )
			{
				correct = true;
			}
		}
		else if (length == 3 || length == 4)
		{
			correct = true;
			
			for (int i = 0; i < length; i++)
			{
                                if( !SWIG_FLOAT_P( RARRAY_PTR(input)[0] ) )
				{
					correct = false;
					break;
				}
			}
		}
	}
	
    return (correct ? 1 : 0);
}
%}

%typemap(typecheck)  
   Ogre::Quaternion, const Ogre::Quaternion, 
   Ogre::Quaternion*, Ogre::Quaternion&,
   const Ogre::Quaternion*, const Ogre::Quaternion& 
%{
	$1 = checkRubyQuaternion($input);	
%}


%{
// Helper method for converting ruby input to a Quaternion
static void convertRubyToQuaternion( VALUE input, Ogre::Quaternion* quat )
{
   if (RARRAY_LEN(input) == 2)
   {
	   VALUE axisVal = rb_ary_entry(input, 0);
	   Ogre::Vector3 axis(
				NUM2DBL(rb_ary_entry(axisVal, 0)), 
				NUM2DBL(rb_ary_entry(axisVal, 1)),
				NUM2DBL(rb_ary_entry(axisVal, 2)));

	   double angle = NUM2DBL(rb_ary_entry(input, 1));
	      
	   quat->FromAngleAxis(Ogre::Degree(angle), axis);
   }
   else if (RARRAY_LEN(input) == 3)
   {
       Ogre::Quaternion rotX, rotY, rotZ;
       rotX.FromAngleAxis(
			Ogre::Degree(NUM2DBL(rb_ary_entry(input, 0))), 
			Ogre::Vector3::UNIT_X);
       rotY.FromAngleAxis(
			Ogre::Degree(NUM2DBL(rb_ary_entry(input, 1))), 
			Ogre::Vector3::UNIT_Y);
       rotZ.FromAngleAxis(
			Ogre::Degree(NUM2DBL(rb_ary_entry(input, 2))), 
			Ogre::Vector3::UNIT_Z);
	   
	   *quat = rotX * rotY * rotZ;
	   quat->normalise();
   }
   else if (RARRAY_LEN(input) == 4)
   {
	   quat->w = NUM2DBL(rb_ary_entry(input, 0));
	   quat->x = NUM2DBL(rb_ary_entry(input, 1));
	   quat->y = NUM2DBL(rb_ary_entry(input, 2));
	   quat->z = NUM2DBL(rb_ary_entry(input, 3));
   }
}
%}

%typemap(in) Ogre::Quaternion, const Ogre::Quaternion
%{
   convertRubyToQuaternion( $input, & $1 );
%}

%typemap(in) Ogre::Quaternion*, Ogre::Quaternion&,
   const Ogre::Quaternion*, const Ogre::Quaternion&
%{
   $1 = new Ogre::Quaternion();  
   convertRubyToQuaternion( $input, $1 );
%}


%{
// Helper method for creating the ruby Array
static VALUE convertQuaternionToRuby( const Ogre::Quaternion* q ) 
{
   Ogre::Degree angle;
   Ogre::Vector3 axis;
   
   q->ToAngleAxis(angle, axis);
   
   VALUE arrAxis = rb_ary_new3( 3, 
       rb_float_new(axis.x), 
       rb_float_new(axis.y), 
       rb_float_new(axis.z) );

   VALUE array = rb_ary_new3( 2, 
       arrAxis, 
       rb_float_new(angle.valueDegrees()) );

   return array;
}
%}

%typemap(out) Ogre::Quaternion, const Ogre::Quaternion 
%{  
   $result = convertQuaternionToRuby( & $1 );
%}

%typemap(out) Ogre::Quaternion*, const Ogre::Quaternion*, const Ogre::Quaternion&, Ogre::Quaternion& 
%{
   $result = convertQuaternionToRuby( $1 );
%} 


%typemap(freearg)  Ogre::Quaternion*, Ogre::Quaternion&,
   const Ogre::Quaternion*, const Ogre::Quaternion& 
"   delete $1;"

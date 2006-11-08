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

/* Typemaps fuer Quaternion.
 * Ein Quaternion wird auf ein Array aus einem 3-elementigen Array 
 * f�r die Rotationsachse und einem Wert f�r den Winkel abgebildet
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
%typemap(freearg)  Ogre::Quaternion*, Ogre::Quaternion&,
   const Ogre::Quaternion*, const Ogre::Quaternion& 
"   delete $1;"




%typemap(out) Ogre::Quaternion, const Ogre::Quaternion {
   Degree angle;
   Vector3 axis;
   
   $1.ToAngleAxis(angle, axis);
   
   VALUE arrAxis = rb_ary_new();
   rb_ary_push(arrAxis, rb_float_new(axis.x));
   rb_ary_push(arrAxis, rb_float_new(axis.y));
   rb_ary_push(arrAxis, rb_float_new(axis.z));
   VALUE rval = rb_ary_new();
   rb_ary_push(rval, arrAxis);
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
   rb_ary_push(rval, arrAxis);
   rb_ary_push(rval, rb_float_new(angle.valueDegrees()));
   
   $result = rval;
} 


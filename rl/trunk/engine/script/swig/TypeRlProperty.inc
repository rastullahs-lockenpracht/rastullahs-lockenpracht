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

%typemap(typecheck) rl::Property, const rl::Property, rl::Property&, const rl::Property&, rl::Property*, const rl::Property* 
{
    if (TYPE($input) == T_FLOAT 
        || TYPE($input) == T_FIXNUM 
	    || TYPE($input) == T_STRING) 
    {
        return 1;
    }
    
    if (TYPE($input == T_ARRAY))
    {
        if (RARRAY($input)->len == 3
            || RARRAY($input)->len == 4)
        {
            return 1;
        }
    }

    return 0;
}

%typemap(in) rl::Property, const rl::Property
"   if (TYPE($input) == T_FLOAT)
    {
	    $1.setValue(Ogre::Real(NUM2DBL($input)));
    }
    else if (TYPE($input) == T_FIXNUM)
    {
        $1.setValue(FIX2INT($input));
    }
    else if (TYPE($input) == T_STRING) 
    {
        $1.setValue(rl::CeGuiString(StringValuePtr($input)));
    }
    else if (TYPE($input) == T_ARRAY)
    {
        if (RARRAY($input)->len == 3)
        {
            Ogre::Vector3 vec;
	        vec.x = NUM2DBL(rb_ary_entry($input, 0));
	        vec.y = NUM2DBL(rb_ary_entry($input, 1));
	        vec.z = NUM2DBL(rb_ary_entry($input, 2));
            $1.setValue(vec);
        }
        else if (RARRAY($input)->len == 4)
        {
            Ogre::Quaternion quat;
            quat.w = NUM2DBL(rb_ary_entry($input, 0));
	        quat.x = NUM2DBL(rb_ary_entry($input, 1));
	        quat.y = NUM2DBL(rb_ary_entry($input, 2));
	        quat.z = NUM2DBL(rb_ary_entry($input, 3));
            $1.setValue(quat);
        }
    }
"

%typemap(in) rl::Property&, const rl::Property&, rl::Property*, const rl::Property*
"   if (TYPE($input) == T_FLOAT)
    {
	    $1 = new rl::Property(Ogre::Real(NUM2DBL($input)));
    }
    else if (TYPE($input) == T_FIXNUM)
    {
        $1 = new rl::Property(FIX2INT($input));
    }
    else if (TYPE($input) == T_STRING) 
    {
        $1 = new rl::Property(rl::CeGuiString(StringValuePtr($input)));
    }
    else if (TYPE($input) == T_ARRAY)
    {
        if (RARRAY($input)->len == 3)
        {
            Ogre::Vector3 vec;
	        vec.x = NUM2DBL(rb_ary_entry($input, 0));
	        vec.y = NUM2DBL(rb_ary_entry($input, 1));
	        vec.z = NUM2DBL(rb_ary_entry($input, 2));
            $1 = new rl::Property(vec);
        }
        else if (RARRAY($input)->len == 4)
        {
            Ogre::Quaternion quat;
            quat.w = NUM2DBL(rb_ary_entry($input, 0));
	        quat.x = NUM2DBL(rb_ary_entry($input, 1));
	        quat.y = NUM2DBL(rb_ary_entry($input, 2));
	        quat.z = NUM2DBL(rb_ary_entry($input, 3));
            $1 = new rl::Property(quat);
        }
    }
"


%typemap(out) rl::Property, const rl::Property
"   if ($1.isString())
    {
        $result = rb_str_buf_new2($1.toString().c_str());
    }
    else if ($1.isInt())
    {
        $result = rb_int_new($1.toInt());
    }
    else if ($1.isReal())
    {
	    $result = rb_float_new($1.toReal());
    }
    else if ($1.isVector())
    {
        Ogre::Vector3 vec = $1.toVector();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_float_new(vec.x));
        rb_ary_push(rval, rb_float_new(vec.y));
        rb_ary_push(rval, rb_float_new(vec.z));
        $result = rval;
    }
    else if ($1.isQuaternion())
    {
        Ogre::Quaternion vec = $1.toQuaternion();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_float_new(vec.w));
        rb_ary_push(rval, rb_float_new(vec.x));
        rb_ary_push(rval, rb_float_new(vec.y));
        rb_ary_push(rval, rb_float_new(vec.z));
        $result = rval;
    }
    else
    {
	    $result = Qnil;
    }"


%typemap(freearg) rl::Property*, rl::Property&, const rl::Property*, const rl::Property&
"   delete $1;"

%typemap(directorout) rl::Property, const rl::Property, rl::Property&, const rl::Property&
"   if (TYPE($1) == T_FLOAT)
    {
	    $result.setValue(Ogre::Real(NUM2DBL($1)));
    }
    else if (TYPE($1) == T_FIXNUM)
    {
        $result.setValue(FIX2INT($1));
    }
    else if (TYPE($1) == T_STRING) 
    {
        $result.setValue(rl::CeGuiString(StringValuePtr($1)));
    }
    else if (TYPE($1) == T_ARRAY)
    {
        if (RARRAY($1)->len == 3)
        {
            Ogre::Vector3 vec;
	        vec.x = NUM2DBL(rb_ary_entry($1, 0));
	        vec.y = NUM2DBL(rb_ary_entry($1, 1));
	        vec.z = NUM2DBL(rb_ary_entry($1, 2));
            $result.setValue(vec);
        }
        else if (RARRAY($1)->len == 4)
        {
            Ogre::Quaternion quat;
            quat.w = NUM2DBL(rb_ary_entry($1, 0));
	        quat.x = NUM2DBL(rb_ary_entry($1, 1));
	        quat.y = NUM2DBL(rb_ary_entry($1, 2));
	        quat.z = NUM2DBL(rb_ary_entry($1, 3));
            $result.setValue(quat);
        }
    }
"

%typemap(directorout) rl::Property*, const rl::Property*
"   if (TYPE($1) == T_FLOAT)
    {
	    $result = new Property(Ogre::Real(NUM2DBL($1)));
    }
    else if (TYPE($1) == T_FIXNUM)
    {
        $result = new Property(FIX2INT($1));
    }
    else if (TYPE($1) == T_STRING) 
    {
        $result = new Property(rl::CeGuiString(StringValuePtr($1)));
    }
    else if (TYPE($1) == T_ARRAY)
    {
        if (RARRAY($1)->len == 3)
        {
            Ogre::Vector3 vec;
	        vec.x = NUM2DBL(rb_ary_entry($1, 0));
	        vec.y = NUM2DBL(rb_ary_entry($1, 1));
	        vec.z = NUM2DBL(rb_ary_entry($1, 2));
            $result = new Property(vec);
        }
        else if (RARRAY($1)->len == 4)
        {
            Ogre::Quaternion quat;
            quat.w = NUM2DBL(rb_ary_entry($1, 0));
	        quat.x = NUM2DBL(rb_ary_entry($1, 1));
	        quat.y = NUM2DBL(rb_ary_entry($1, 2));
	        quat.z = NUM2DBL(rb_ary_entry($1, 3));
            $result = new Property(quat);
        }
    }
"

%typemap(directorin) rl::Property, const rl::Property, rl::Property&, const rl::Property&
"   if ($1.isString())
    {
        $input = rb_str_buf_new2($1.toString().c_str());
    }
    else if ($1.isInt())
    {
        $input = rb_int_new($1.toInt());
    }
    else if ($1.isReal())
    {
	    $input = rb_float_new($1.toReal());
    }
    else if ($1.isVector())
    {
        Ogre::Vector3 vec = $1.toVector();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_float_new(vec.x));
        rb_ary_push(rval, rb_float_new(vec.y));
        rb_ary_push(rval, rb_float_new(vec.z));
        $input = rval;
    }
    else if ($1.isQuaternion())
    {
        Ogre::Quaternion vec = $1.toQuaternion();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_float_new(vec.w));
        rb_ary_push(rval, rb_float_new(vec.x));
        rb_ary_push(rval, rb_float_new(vec.y));
        rb_ary_push(rval, rb_float_new(vec.z));
        $input = rval;
    }
    else
    {
	    $input = Qnil;
    }
"

%typemap(directorin) rl::Property*, const rl::Property*
"   if ($1->isString())
    {
        $input = rb_str_buf_new2($1->toString().c_str());
    }
    else if ($1->isInt())
    {
        $input = rb_int_new($1->toInt());
    }
    else if ($1->isReal())
    {
	    $input = rb_float_new($1->toReal());
    }
    else if ($1->isVector())
    {
        Ogre::Vector3 vec = $1->toVector();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_float_new(vec.x));
        rb_ary_push(rval, rb_float_new(vec.y));
        rb_ary_push(rval, rb_float_new(vec.z));
        $input = rval;
    }
    else if ($1->isQuaternion())
    {
        Ogre::Quaternion vec = $1->toQuaternion();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_float_new(vec.w));
        rb_ary_push(rval, rb_float_new(vec.x));
        rb_ary_push(rval, rb_float_new(vec.y));
        rb_ary_push(rval, rb_float_new(vec.z));
        $input = rval;
    }
    else
    {
	    $input = Qnil;
    }
"
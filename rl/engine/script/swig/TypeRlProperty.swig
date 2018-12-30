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

%{
namespace rl {

rl::Property convertValueToProperty(VALUE input)
{
	rl::Property rval;
	if (TYPE(input) == T_FLOAT)
    {
	    rval.setValue(Ogre::Real(NUM2DBL(input)));
    }
    else if (TYPE(input) == T_FIXNUM)
    {
        rval.setValue(FIX2INT(input));
    }
    else if (TYPE(input) == T_FIXNUM)
    {
        rval.setValue(FIX2INT(input));
    }
    else if (TYPE(input) == T_TRUE)
    {
        rval.setValue(true);
    }
    else if (TYPE(input) == T_FALSE)
    {
        rval.setValue(false);
    }
    else if (TYPE(input) == T_STRING) 
    {
        rval.setValue(rl::CeGuiString(StringValuePtr(input)));
    }
    else if (TYPE(input) == T_ARRAY)
    {
		int length = RARRAY_LEN(input);
		if (length > 0)
		{
			VALUE firstVal = rb_ary_entry(input, 0);
            if (length == 2
                && TYPE(firstVal) == T_FIXNUM)
            {
                IntPair pair;
                pair.first = NUM2INT(rb_ary_entry(input, 0));
                pair.second = NUM2INT(rb_ary_entry(input, 1));
                rval.setValue(pair);
            }
			else if (length == 3 
				&& TYPE(firstVal) == T_FIXNUM
                && TYPE(rb_ary_entry(input, 1)) == T_FIXNUM
                && TYPE(rb_ary_entry(input, 2)) == T_FIXNUM)
            {
                Tripel<int> tripel;
                tripel.first = NUM2INT(rb_ary_entry(input, 0));
                tripel.second = NUM2INT(rb_ary_entry(input, 1));
                tripel.third = NUM2INT(rb_ary_entry(input, 2));
                rval.setValue(tripel);
            }
			else if (length == 3 
				&& (TYPE(firstVal) == T_FLOAT || TYPE(firstVal) == T_FIXNUM))
			{
				Ogre::Vector3 vec;
				vec.x = NUM2DBL(rb_ary_entry(input, 0));
				vec.y = NUM2DBL(rb_ary_entry(input, 1));
				vec.z = NUM2DBL(rb_ary_entry(input, 2));
				rval.setValue(vec);
			}
			else if (length == 4 
				&& (TYPE(firstVal) == T_FLOAT || TYPE(firstVal) == T_FIXNUM))
			{
				Ogre::Quaternion quat;
				quat.w = NUM2DBL(rb_ary_entry(input, 0));
				quat.x = NUM2DBL(rb_ary_entry(input, 1));
				quat.y = NUM2DBL(rb_ary_entry(input, 2));
				quat.z = NUM2DBL(rb_ary_entry(input, 3));
				rval.setValue(quat);
			}
			else // Array of Properties
			{
				std::vector<rl::Property> vec;
				for (int idx = 0; idx < length; ++idx)
				{
					vec.push_back(rl::convertValueToProperty(rb_ary_entry(input, idx)));
				}
				rval.setValue(vec);
			}
        }
    }
    else if (TYPE(input) == T_HASH)
    {
        PropertyMap pmap;
        VALUE keys = rb_funcall(input,rb_intern("keys"),0);
        unsigned int size = RARRAY_LEN(keys);
        for (unsigned int i=0; i<size; i++) 
        {
            VALUE key = RARRAY_PTR(keys)[i];
            VALUE val = rb_hash_aref(input, key);
            pmap[rl::CeGuiString(StringValuePtr(key))] 
                = rl::convertValueToProperty(val);
        }
        rval.setValue(pmap);
    }
    
    return rval;
}

VALUE convertPropertyToValue(rl::Property input)
{
	if (input.isString())
    {
        return rb_str_buf_new2(input.toString().c_str());
    }
    else if (input.isInt())
    {
        return rb_int_new(input.toInt());
    }
    else if (input.isReal())
    {
	    return rb_float_new(input.toReal());
    }
    else if (input.isBool())
    {
        bool boolValue = input.toBool();
        if (boolValue)
        {
            return Qtrue;
        }
        else
        {
            return Qfalse;
        }
    }
    else if (input.isVector3())
    {
        Ogre::Vector3 vec = input.toVector3();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_float_new(vec.x));
        rb_ary_push(rval, rb_float_new(vec.y));
        rb_ary_push(rval, rb_float_new(vec.z));
        return rval;
    }
    else if (input.isQuaternion())
    {
        Ogre::Quaternion vec = input.toQuaternion();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_float_new(vec.w));
        rb_ary_push(rval, rb_float_new(vec.x));
        rb_ary_push(rval, rb_float_new(vec.y));
        rb_ary_push(rval, rb_float_new(vec.z));
        return rval;
    }
    else if (input.isArray())
    {
		PropertyArray vec = input.toArray();
		VALUE rval = rb_ary_new();
		for (PropertyArray::const_iterator it = vec.begin();
			it != vec.end(); ++it)
		{
			rb_ary_push(rval, rl::convertPropertyToValue(*it));
		}
		return rval;
    }
    else if (input.isMap())
    {
        PropertyMap pmap = input.toMap();
        VALUE rval = rb_hash_new();
        for (PropertyMap::const_iterator it = pmap.begin(); 
            it != pmap.end(); ++it) 
        {
            rb_hash_aset(
                rval,
                rb_str_buf_new2(it->first.c_str()),
                rl::convertPropertyToValue(it->second));
        }
        return rval;
    }
    else if (input.isIntPair())
    {
        IntPair pair = input.toIntPair();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_int_new(pair.first));
        rb_ary_push(rval, rb_int_new(pair.second));
        return rval;
    }
    else if (input.isIntTriple())
    {
        Tripel<int> triple = input.toIntTriple();
        VALUE rval = rb_ary_new();
        rb_ary_push(rval, rb_int_new(triple.first));
        rb_ary_push(rval, rb_int_new(triple.second));
        rb_ary_push(rval, rb_int_new(triple.third));
        return rval;
    }

    return Qnil;
}

}
%}


%typemap(typecheck) rl::Property, const rl::Property, rl::Property&, const rl::Property&, rl::Property*, const rl::Property* 
"
    if (TYPE($input) == T_FLOAT 
        || TYPE($input) == T_FIXNUM 
	    || TYPE($input) == T_STRING
	    || TYPE($input) == T_TRUE 
	    || TYPE($input) == T_FALSE
	    || TYPE($input) == T_HASH
	    || TYPE($input) == T_ARRAY) 
    {
        return 1;
    }
    
    return 0;
"

%typemap(in) rl::Property, const rl::Property
"   
	$1 = rl::convertValueToProperty($input);
"

%typemap(in) rl::Property&, const rl::Property&, rl::Property*, const rl::Property*
"   
	$1 = new rl::Property(rl::convertValueToProperty($input));
"


%typemap(out) rl::Property, const rl::Property
"   
	$result = rl::convertPropertyToValue($1);
"


%typemap(freearg) rl::Property*, rl::Property&, const rl::Property*, const rl::Property&
"   delete $1;"

%typemap(directorout) rl::Property, const rl::Property, rl::Property&, const rl::Property&
"   $result = rl::convertValueToProperty($1);"

%typemap(directorout) rl::Property*, const rl::Property*
"   
	$result = new rl::Property(rl::convertValueToProperty($1));
"

%typemap(directorin) rl::Property, const rl::Property, rl::Property&, const rl::Property&
"   
	$input = rl::convertPropertyToValue($1);
"

%typemap(directorin) rl::Property*, const rl::Property*
"   	
	$input = rl::convertPropertyToValue(*$1);
"
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

%typemap(typecheck) rl::Property, const rl::Property, rl::Property, const rl::Property& 
{
    if (TYPE($input) == T_FLOAT 
        || TYPE($input) == T_FIXNUM 
	|| TYPE($input) == T_STRING) 
    {
        return 1;
    }

    return 0;
}

%typemap(in) rl::Property, const rl::Property
"   if (TYPE($input) == T_FLOAT)
    {
	$1.setValue(NUM2DBL($input));
    }
    else if (TYPE($input) == T_FIXNUM)
    {
        $1.setValue(FIX2INT($input));
    }
    else if (TYPE($input) == T_STRING) 
    {
        $1.setValue(StringValuePtr($input));
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
    else
    {
	    $result = Qnil;
    }"


%typemap(freearg) rl::Property*, rl::Property&, const rl::Property*, const rl::Property&
"   delete $1;"


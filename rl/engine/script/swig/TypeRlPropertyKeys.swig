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

namespace rl
{

//Conversion functions for PropertyKey (std::set<CeGuiString>)
rl::PropertyKeys convertValueToPropertyKeys(VALUE input)
{
    int length = RARRAY_LEN(input);
    rl::PropertyKeys keys;
    for (int idx = 0; idx < length; ++idx)
    {
	    VALUE cur = rb_ary_entry(input, idx);
	    keys.insert(rl::CeGuiString(StringValuePtr(cur)));
    }

    return keys;
}

VALUE convertPropertyKeysToValue(PropertyKeys input)
{
    VALUE rval = rb_ary_new();
    PropertyKeys::const_iterator end = input.end();
    for (PropertyKeys::const_iterator it = input.begin(); it != end; ++it)
    {
        rb_ary_push(rval, rb_str_buf_new2(it->c_str()));
    }
    return rval;
}

}

%}


%typemap(typecheck) rl::PropertyKeys, const rl::PropertyKeys 
"
    if (TYPE($input) == T_ARRAY) 
    {
        return 1;
    }
    
    return 0;
"

%typemap(in) rl::PropertyKeys, const rl::PropertyKeys
"   
	$1 = rl::convertValueToPropertyKeys($input);
"

%typemap(out) rl::PropertyKeys, const rl::PropertyKeys
"   
	$result = rl::convertPropertyKeysToValue($1);
"

%typemap(directorout) rl::PropertyKeys, const rl::PropertyKeys
"   
	$result = rl::convertValueToPropertyKeys($1);
"

%typemap(directorin) rl::PropertyKeys, const rl::PropertyKeys
"   
	$input = rl::convertPropertyKeysToValue($1);
"

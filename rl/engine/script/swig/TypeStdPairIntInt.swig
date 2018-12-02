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

%typecheck(SWIG_TYPECHECK_INT32_ARRAY) 
	std::pair<int,int>*, std::pair<int,int>&, 
	const std::pair<int,int>*, const std::pair<int,int>&, 
	std::pair<int,int>, const std::pair<int,int>
"   $1 = TYPE($input) == T_ARRAY && RARRAY_LEN($input) == 2 ? 1 : 0; "


%typemap(in) std::pair<int,int>*, std::pair<int,int>&,
   const std::pair<int,int>*, const std::pair<int,int>&
"
	$1 = new std::pair<int, int>();
	$1->first = NUM2INT(rb_ary_entry($input, 0)), 
	$1->second = NUM2INT(rb_ary_entry($input, 1));
"

%typemap(freearg) std::pair<int,int>*, std::pair<int,int>&, 
const std::pair<int,int>*, const std::pair<int,int>&
   "delete $1;"

%typemap(in) std::pair<int,int>, const std::pair<int,int>
"
	$1.first = NUM2INT(rb_ary_entry($input, 0)), 
	$1.second = NUM2INT(rb_ary_entry($input, 1));
"

%typemap(out) std::pair<int,int>, const std::pair<int,int>
"
	$result = rb_ary_new();
	rb_ary_push($result, rb_int_new($1.first));
	rb_ary_push($result, rb_int_new($1.second));
"

%typemap(out) std::pair<int,int>*, std::pair<int,int>&,
   const std::pair<int,int>*, const std::pair<int,int>&
"
	$result = rb_ary_new();
	rb_ary_push($result, rb_int_new($1->first));
	rb_ary_push($result, rb_int_new($1->second));
"

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

/* Wrapping rl::CeGuiStrings to ruby and back 
 for rl::CeGuiString
*/
%typemap(typecheck) rl::CeGuiString, const rl::CeGuiString, rl::CeGuiString*, rl::CeGuiString&, const rl::CeGuiString*, const rl::CeGuiString& = char *;

%typemap(in) rl::CeGuiString, const rl::CeGuiString 
	"Check_Type($input, T_STRING);
    $1 = rl::CeGuiString(
		reinterpret_cast<CEGUI::utf8*>(StringValuePtr($input)));"

%typemap(out) rl::CeGuiString, const rl::CeGuiString 
     "$result = rb_str_buf_new2($1.c_str());"


%typemap(in) rl::CeGuiString*, rl::CeGuiString&, const rl::CeGuiString*, const rl::CeGuiString& 
    "Check_Type($input, T_STRING);
    $1 = new rl::CeGuiString(
		reinterpret_cast<CEGUI::utf8*>(StringValuePtr($input)));"

%typemap(freearg) rl::CeGuiString*, rl::CeGuiString&, const rl::CeGuiString*, const rl::CeGuiString&
   "delete $1;"

%typemap(out) rl::CeGuiString*, rl::CeGuiString&,  const rl::CeGuiString*, const rl::CeGuiString& 
     "$result = rb_str_buf_new2($1->c_str());"

%typemap(varout) rl::CeGuiString, const rl::CeGuiString 
     "$result = rb_str_buf_new2($1.c_str());"

%typemap(directorin) rl::CeGuiString, const rl::CeGuiString &, rl::CeGuiString & 
	"$input = rb_str_buf_new2($1.c_str());"

%typemap(directorin) rl::CeGuiString *, const rl::CeGuiString * 
	"$input = rb_str_buf_new2($1->c_str());"

%typemap(directorout) rl::CeGuiString, const rl::CeGuiString 
    "$result = rl::CeGuiString(reinterpret_cast<CEGUI::utf8*>(StringValuePtr($input)));"

%typemap(directorout) const rl::CeGuiString &, rl::CeGuiString&  
    "$result = new rl::CeGuiString();
     $result->append(reinterpret_cast<CEGUI::utf8*>(StringValuePtr($input)));"

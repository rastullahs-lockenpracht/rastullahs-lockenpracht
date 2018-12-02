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
void convertRubyToCeGuiStringVector(VALUE input, rl::CeGuiStringVector& vec)
{
  Check_Type(input, T_ARRAY);
  int len = RARRAY_LEN(input);
  for (int i=0; i!=len; i++) 
  {
    VALUE inst = rb_ary_entry(input, i);
    vec.push_back(rl::CeGuiString(StringValuePtr(inst)));
  }
}

VALUE convertCeGuiStringVectorToRuby(const rl::CeGuiStringVector& input)
{
  VALUE arr = rb_ary_new2(input.size()); 
  for (rl::CeGuiStringVector::const_iterator i = input.begin(), iend = input.end() ; i!=iend; i++ )
  {
    rb_ary_push(arr, rb_str_new2(i->c_str()));
  }
  return arr;
}
%}

%typemap(freearg) rl::CeGuiStringVector &, const rl::CeGuiStringVector & 
"  delete $1;"


%typemap(directorin) rl::CeGuiStringVector &, const rl::CeGuiStringVector & 
%{
  $result = convertCeGuiStringVectorToRuby(*$1); 
%}

%typemap(directorin) rl::CeGuiStringVector, const rl::CeGuiStringVector
%{
  $result = convertCeGuiStringVectorToRuby($1); 
%}

%typemap(directorout) rl::CeGuiStringVector , const rl::CeGuiStringVector
%{
  convertRubyToCeGuiStringVector($input, $result);
%}

%typemap(directorout) rl::CeGuiStringVector &, const rl::CeGuiStringVector& 
%{
  $result = new rl::CeGuiStringVector();
  convertRubyToCeGuiStringVector($input, * $result);
%}

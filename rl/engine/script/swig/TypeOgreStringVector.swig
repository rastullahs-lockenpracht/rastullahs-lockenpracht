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
void convertRubyToOgreStringVector(VALUE input, Ogre::StringVector& vec)
{
  Check_Type(input, T_ARRAY);
  int len = RARRAY_LEN(input);
  for (int i=0; i!=len; i++) 
  {
    VALUE inst = rb_ary_entry(input, i);
    vec.push_back(Ogre::String(StringValuePtr(inst)));
  }
}

VALUE convertOgreStringVectorToRuby(const Ogre::StringVector& vec)
{
  VALUE arr = rb_ary_new2(vec.size()); 
  for (Ogre::StringVector::const_iterator i = vec.begin(), iend = vec.end() ; i!=iend; i++ )
  {
    rb_ary_push(arr, rb_str_new2(i->c_str()));
  }
  return arr;
}

static int checkRubyOgreStringVector( VALUE input ) 
{
	bool correct = false;

	if (TYPE(input) == T_ARRAY)
	{
        correct = true;
	}
	
    return (correct ? 1 : 0);
}

%}


%typemap(typecheck)  
   Ogre::StringVector, const Ogre::StringVector, 
   Ogre::StringVector*, Ogre::StringVector&,
   const Ogre::StringVector*, const Ogre::StringVector& 
%{
	$1 = checkRubyOgreStringVector($input);	
%}

%typemap(freearg) Ogre::StringVector &, const Ogre::StringVector & 
    "delete $1;"

%typemap(out) Ogre::StringVector, const Ogre::StringVector 
%{
  $result = convertOgreStringVectorToRuby($1); 
%}
%typemap(out) Ogre::StringVector&, const Ogre::StringVector&, Ogre::StringVector*, const Ogre::StringVector* 
%{
  $result = convertOgreStringVectorToRuby(*$1); 
%}

%typemap(in) Ogre::StringVector , const Ogre::StringVector 
%{
  convertRubyToOgreStringVector($input, $1);
%}
%typemap(in) Ogre::StringVector&, const Ogre::StringVector&, Ogre::StringVector*, const Ogre::StringVector*
%{
    $result = new Ogre::StringVector();
    convertRubyToOgreStringVector($input, *$1);
%}

%typemap(directorin) Ogre::StringVector &, const Ogre::StringVector &
%{
  $result = convertOgreStringVectorToRuby(*$1); 
%}
%typemap(directorin) Ogre::StringVector, const Ogre::StringVector 
%{
  $result = convertOgreStringVectorToRuby($1);
%}

%typemap(directorout) Ogre::StringVector , const Ogre::StringVector 
%{
  convertRubyToOgreStringVector($input, $result);
%}

%typemap(directorout) Ogre::StringVector *, const Ogre::StringVector* , Ogre::StringVector &, const Ogre::StringVector& 
%{
  $result = new Ogre::StringVector();
  convertRubyToOgreStringVector($input, *$result);
%}

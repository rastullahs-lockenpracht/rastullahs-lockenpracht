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
void convertRubyToRlGameObjectList(VALUE input, rl::GameObjectList& vec)
{
  Check_Type(input, T_ARRAY);
  int len = RARRAY_LEN(input);
  for (int i=0; i!=len; i++) 
  {
    VALUE inst = rb_ary_entry(input, i);
    void* go = NULL;
    int result = SWIG_ConvertPtr(inst, &go, SWIGTYPE_p_rl__GameObject, 0 |  0 );
    if (SWIG_IsOK(result)) {
        vec.push_back(static_cast<rl::GameObject*>(go));
    }
  }
}

VALUE convertGameObjectListToRuby(const rl::GameObjectList& vec)
{
  VALUE arr = rb_ary_new2(vec.size()); 
  for (rl::GameObjectList::const_iterator i = vec.begin(), iend = vec.end() ; i!=iend; i++ )
  {
    rl::GameObject* go = *i;
    VALUE goVal = getDirectorInScriptObject(dynamic_cast<Swig::Director*>(go), go, SWIGTYPE_p_rl__GameObject);
    rb_ary_push(arr, goVal);
  }
  return arr;
}

static int checkRubyGameObjectList( VALUE input ) 
{
	bool correct = false;

	if (TYPE(input) == T_ARRAY)
	{
        correct = true;
	}
	
    return (correct ? 1 : 0);
}

void convertRubyToCreatureList(VALUE input, rl::CreatureList& vec)
{
  Check_Type(input, T_ARRAY);
  int len = RARRAY_LEN(input);
  for (int i=0; i!=len; i++) 
  {
    VALUE inst = rb_ary_entry(input, i);
    void* go = NULL;
    int result = SWIG_ConvertPtr(inst, &go, SWIGTYPE_p_rl__Creature, 0 |  0 );
    if (SWIG_IsOK(result)) {
        vec.push_back(static_cast<rl::Creature*>(go));
    }
  }
}

VALUE convertCreatureListToRuby(const rl::CreatureList& vec)
{
  VALUE arr = rb_ary_new2(vec.size()); 
  for (rl::CreatureList::const_iterator i = vec.begin(), iend = vec.end() ; i!=iend; i++ )
  {
    rl::Creature* go = *i;
    VALUE goVal = getDirectorInScriptObject(dynamic_cast<Swig::Director*>(go), go, SWIGTYPE_p_rl__Creature);
    rb_ary_push(arr, goVal);
  }
  return arr;
}

%}


%typemap(typecheck)  
   rl::GameObjectList, const rl::GameObjectList, 
   rl::GameObjectList*, rl::GameObjectList&,
   const rl::GameObjectList*, const rl::GameObjectList& 
%{
	$1 = checkRubyGameObjectList($input);	
%}

%typemap(freearg) rl::GameObjectList &, const rl::GameObjectList & 
    "delete $1;"

%typemap(out) rl::GameObjectList, const rl::GameObjectList 
%{
  $result = convertGameObjectListToRuby($1); 
%}
%typemap(out) rl::GameObjectList&, const rl::GameObjectList&, rl::GameObjectList*, const rl::GameObjectList* 
%{
  $result = convertGameObjectListToRuby(*$1); 
%}

%typemap(in) rl::GameObjectList , const rl::GameObjectList 
%{
  convertRubyToGameObjectList($input, $1);
%}
%typemap(in) rl::GameObjectList&, const rl::GameObjectList&, rl::GameObjectList*, const rl::GameObjectList*
%{
    $1 = new rl::GameObjectList();
    convertRubyToGameObjectList($input, *$1);
%}

%typemap(directorin) rl::GameObjectList &, const rl::GameObjectList &
%{
  $result = convertGameObjectListToRuby(*$1); 
%}
%typemap(directorin) rl::GameObjectList, const rl::GameObjectList 
%{
  $result = convertGameObjectListToRuby($1);
%}

%typemap(directorout) rl::GameObjectList , const rl::GameObjectList 
%{
  convertRubyToGameObjectList($input, $result);
%}

%typemap(directorout) rl::GameObjectList *, const rl::GameObjectList* , rl::GameObjectList &, const rl::GameObjectList& 
%{
  $result = new rl::GameObjectList();
  convertRubyToGameObjectList($input, *$result);
%}

%typemap(typecheck)  
   rl::CreatureList, const rl::CreatureList, 
   rl::CreatureList*, rl::CreatureList&,
   const rl::CreatureList*, const rl::CreatureList& 
%{
	$1 = checkRubyGameObjectList($input);	
%}

%typemap(freearg) rl::CreatureList &, const rl::CreatureList & 
    "delete $1;"

%typemap(out) rl::CreatureList, const rl::CreatureList 
%{
  $result = convertCreatureListToRuby($1); 
%}
%typemap(out) rl::CreatureList&, const rl::CreatureList&, rl::CreatureList*, const rl::CreatureList* 
%{
  $result = convertCreatureListToRuby(*$1); 
%}

%typemap(in) rl::CreatureList , const rl::CreatureList 
%{
  convertRubyToCreatureList($input, $1);
%}
%typemap(in) rl::CreatureList&, const rl::CreatureList&, rl::CreatureList*, const rl::CreatureList*
%{
    $1 = new rl::CreatureList();
    convertRubyToCreatureList($input, *$1);
%}

%typemap(directorin) rl::CreatureList &, const rl::CreatureList &
%{
  $result = convertCreatureListToRuby(*$1); 
%}
%typemap(directorin) rl::CreatureList, const rl::CreatureList 
%{
  $result = convertCreatureListToRuby($1);
%}

%typemap(directorout) rl::CreatureList , const rl::CreatureList 
%{
  convertRubyToCreatureList($input, $result);
%}

%typemap(directorout) rl::CreatureList *, const rl::CreatureList* , rl::CreatureList &, const rl::CreatureList& 
%{
  $result = new rl::CreatureList();
  convertRubyToCreatureList($input, *$result);
%}

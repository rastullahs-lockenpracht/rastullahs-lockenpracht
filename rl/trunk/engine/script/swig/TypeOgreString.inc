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

/* Wrapping Strings to ruby and back 
 for String, const String, String&, String*, const String*, const String&
*/
%typemap(typecheck) Ogre::String, const Ogre::String & = char *;

%typemap(in) Ogre::String, const Ogre::String {
    Check_Type($input, T_STRING);
    $1 = Ogre::String(StringValuePtr($input));
}
%typemap(out) Ogre::String, const Ogre::String {
     $result = rb_str_buf_new2($1.c_str());
}
%typemap(in) Ogre::String*, Ogre::String&, const Ogre::String*, const Ogre::String& {
    Check_Type($input, T_STRING);
    $1 = new Ogre::String(StringValuePtr($input));
}
%typemap(freearg) Ogre::String*, Ogre::String&, const Ogre::String*, const Ogre::String& {
   delete $1;
}
%typemap(out) Ogre::String*, Ogre::String&, const Ogre::String*, const Ogre::String& {
     $result = rb_str_buf_new2($1->c_str());
}
%typemap(varout) Ogre::String, const Ogre::String {
     $result = rb_str_buf_new2($1.c_str());
}

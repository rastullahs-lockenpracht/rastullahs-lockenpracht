/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

/* Wrapping Real to ruby and back 
 for Real, const Real, Real&, Real*, const Real*, const Real&
*/
%typemap(typecheck) Ogre::Real, const Ogre::Real& = double;
    
%typemap(in) Ogre::Real, const Ogre::Real 
"   $1 = NUM2DBL($input);"

%typemap(out) Ogre::Real, const Ogre::Real
"   $result = rb_float_new($1);"

%typemap(in) Ogre::Real*, Ogre::Real&, const Ogre::Real*, const Ogre::Real&
"   $1 = NUM2DBL($input);"

%typemap(out) Ogre::Real*, Ogre::Real&,  const Ogre::Real*, const Ogre::Real&
"   $result = rb_float_new(*$1);"

%typemap(freearg) Ogre::Real*, Ogre::Real&,  const Ogre::Real*, const Ogre::Real&
"   delete $1;"



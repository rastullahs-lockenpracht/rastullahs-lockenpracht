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

/* Radian / Degree all Ruby Values are interpreted as DEGREE! */
%typemap(typecheck) Ogre::Radian, const Ogre::Radian& = double;

%typemap(in) Ogre::Radian, const Ogre::Radian, Ogre::Radian&, const Ogre::Radian& 
%{
    if (TYPE($input) == T_FLOAT) {
                $1 = Ogre::Degree(NUM2DBL($input));
    } else if (TYPE($input) == T_FIXNUM) {
		$1 = Ogre::Degree(FIX2INT($input));
    } else {
        throw Swig::DirectorTypeMismatchException("float or fixnum expected");
    }
%}

%typemap(in) Ogre::Radian*, const Ogre::Radian*, Ogre::Radian&, const Ogre::Radian&
%{
    if (TYPE($input) == T_FLOAT) {
                $1 = new Ogre::Radian(Ogre::Degree(NUM2DBL($input)));
    } else if (TYPE($input) == T_FIXNUM) {
		$1 = new Ogre::Radian(Ogre::Degree(FIX2INT($input)));
    } else {
        throw Swig::DirectorTypeMismatchException("float or fixnum expected");
    }
%}

%typemap(freearg) Ogre::Radian*, const Ogre::Radian*, Ogre::Radian&, const Ogre::Radian& 
    "delete $1;"

%typemap(out) Ogre::Radian, const Ogre::Radian 
    "$result = rb_float_new($1.valueDegrees());"

%typemap(out) Ogre::Radian&, const Ogre::Radian& 
    "$result = rb_float_new($1->valueDegrees());"




/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __RastullahPrerequisites_H__
#define __RastullahPrerequisites_H__

#ifdef HAVE_CONFIG_H
#   undef PACKAGE_NAME
#   undef PACKAGE
#   undef PACKAGE_BUGREPORT
#   undef PACKAGE_STRING
#   undef PACKAGE_TARNAME
#   undef PACKAGE_VERSION
#   include "ruby.h"
#   include "FixRubyHeaders.h"
#endif

#ifdef HAVE_CONFIG_H
#   undef PACKAGE_NAME
#   undef PACKAGE
#   undef PACKAGE_BUGREPORT
#   undef PACKAGE_STRING
#   undef PACKAGE_TARNAME
#   undef PACKAGE_VERSION
#endif

#include <Ogre.h>

#ifdef HAVE_CONFIG_H
#   undef PACKAGE_NAME
#   undef PACKAGE
#   undef PACKAGE_BUGREPORT
#   undef PACKAGE_STRING
#   undef PACKAGE_TARNAME
#   undef PACKAGE_VERSION
#   include "config.h" 
#endif


#if OGRE_PLATFORM != PLATFORM_WIN32
#   define _snprintf snprintf
template<class T1, class T2>
bool max(T1 m1, T2 m2) { return (m1 < m2)?m2:m1; }
#endif

#if !defined(RL_LONGLONG)
#   if defined(_MSC_VER) && (_MSC_VER < 1300)
#       define RL_LONGLONG _int64
#   else
#       define RL_LONGLONG long long
#   endif
#endif

#if defined(_MSC_VER) && (_MSC_VER > 1300)   // MS Visual Studio 2005
// deprecated-Warnung aus, denn fast alle Stringfunktionen sind unsicher
// signed / unsigned auf 1x
#  pragma warning(disable : 4996; once : 4018) 
#endif

#endif

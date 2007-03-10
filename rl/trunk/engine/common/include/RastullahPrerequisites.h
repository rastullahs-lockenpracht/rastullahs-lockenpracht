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

#ifndef __RastullahPrerequisites_H__
#define __RastullahPrerequisites_H__

#define OGRE_MEMORY_STRESS_TEST 1

#undef max
#undef min

#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreVector3.h>

#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
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
// signed / unsigned auf 1x
#    pragma warning(once : 4018)
// Sichere Versionen der Stringfunktionen benutzen
#    if !defined(_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
#        define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#    endif
#endif

#endif

/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __CommonPrerequisites_H__
#define __CommonPrerequisites_H__

#include "RastullahPrerequisites.h"

#undef _assert

#if OGRE_PLATFORM == PLATFORM_WIN32
// Export control
#   if defined( RLCOMMON_EXPORTS )
#       define _RlCommonExport __declspec( dllexport )
#   else
#       define _RlCommonExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlCommonExport
#endif

#endif

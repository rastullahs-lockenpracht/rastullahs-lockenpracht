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

#ifndef __UiPrerequisites_H__
#define __UiPrerequisites_H__

#include <xercesc/util/XMemory.hpp>	// Muss vor Ogre stehen (zumindest für VS)

#undef _assert

#if OGRE_PLATFORM == PLATFORM_WIN32
// Export control
#   if defined( RLUI_EXPORTS )
#       define _RlUiExport __declspec( dllexport )
#   else
#       define _RlUiExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlUiExport
#endif

#include <vector>
#include "FixRubyHeaders.h"
#include <CEGUIString.h>

#include "RastullahPrerequisites.h"

typedef CEGUI::String CeGuiString;

namespace rl
{
    typedef std::vector<CeGuiString> CeGuiStringVector;
}

#endif

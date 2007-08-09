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
#ifndef __DialogPrerequisites_H__
#define __DialogPrerequisites_H__

// Common is only needed for the CeGuiString typedef
#include "CommonPrerequisites.h"

#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#   if defined( RLDIALOG_EXPORTS )
#       define _RlDialogExport __declspec( dllexport )
#   else
#       define _RlDialogExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlDialogExport
#endif

#pragma warning (disable : 4251)

#endif


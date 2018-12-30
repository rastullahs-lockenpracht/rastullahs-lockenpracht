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

/*
 * This file is used for precompiled header generation. You have to
 * include it in all cpp files which have "Use Precompiled Header"
 * set in their C/C++ properties. It shall never be included in other
 * headers.
 *
 * The content of this file should be limited to the following include
 * types:
 *   - stdlib headers
 *   - stl headers
 *   - all types of external dependencies which will never be changed by us
 *
 * Every sub project should have it's own copy of this file and may have
 * project specific modifications
 */

#ifndef __stdinc_h__
#define __stdinc_h__

#ifdef RL_USE_PCH

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <iostream>
#include <sstream>

#include <algorithm>
#include <functional>
#include <utility>

#include <Ogre.h>

#include <CEGUI/Logger.h>
#include <CEGUI/String.h>

#include <boost/any.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <Windows.h>
#endif

#endif // RL_USE_PCH
#endif

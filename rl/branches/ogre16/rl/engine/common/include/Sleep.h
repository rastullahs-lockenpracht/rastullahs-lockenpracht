/* Sleep.h - Plattformunabhaengigkeit für sleep
 * (C) 2003-2008. Team Pantheon. www.team-pantheon.de
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
 *  along with this program; if not you can get it here:
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm
 */


#include "RastullahPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "Windows.h"
#endif
	

namespace rl {
/**
 * @param sleeptime Die Zeit in Millisekunden.
 * Legen den aktuellen Thread für sleeptime Millisekungen schlafen.
 */
inline void msleep(unsigned long sleeptime)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    Sleep(sleeptime);
#else
    usleep(sleeptime * 1000);
#endif
}

}

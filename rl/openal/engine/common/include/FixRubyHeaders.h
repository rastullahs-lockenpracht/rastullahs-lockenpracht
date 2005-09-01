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

/**
 * Einige Undefines, für unfreundliche defines in den Ruby-Headers.
 * Dort werden viele C-Funktionen überdeckt, die in Ogre aber benötigt werden.
 * Diese Datei muss nach einem Ruby-Header inkludiert werden, möglicherweise auch
 * noch davor, um vorherige Makros zu löschen.
 */
#undef close
#undef bind
#undef select
#undef shutdown
#undef snprintf
#undef vsnprintf
#undef Sleep
#undef sleep

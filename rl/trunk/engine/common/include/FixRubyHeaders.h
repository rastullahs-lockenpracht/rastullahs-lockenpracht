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

/**
 * Einige Undefines, für unfreundliche defines in den Ruby-Headers.
 * Dort werden viele C-Funktionen überdeckt, die in Ogre aber benötigt werden.
 * Diese Datei muss nach einem Ruby-Header inkludiert werden.
 */
#ifndef __FIXRUBYHEADERS_H__
#define __FIXRUBYHEADERS_H__

#undef close
#undef bind
#undef select
#undef shutdown
#undef snprintf
#undef vsnprintf
#undef Sleep
#undef sleep

#endif

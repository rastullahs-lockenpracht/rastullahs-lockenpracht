/* XtimeOperator.h - Operatoren für xtime aus Boost
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License as published by
 *  the .
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not
 */

/** Operatoren für Boosts xtime-Klasse. */

#include <boost/thread/xtime.hpp>
#include "RastullahPrerequisites.h"
#include <iostream>

using namespace boost;

/** Ermittle die Zeitdifferenz zwischen a und b.
 * @param a
 * @param b
 * @return Die Zeitdifferenz.
 * @author JoSch
 * @date 09-11-2004
 */
unsigned RL_LONGLONG operator-(const xtime &a, const xtime &b)
{
    unsigned RL_LONGLONG x = (a.nsec / 1000 + a.sec * 1000 * 1000) - (b.nsec / 1000 + b.sec * 1000 * 1000);
    return x;
}

/** Vergleiche die Zeit a und b.
 * @param a
 * @param b
 * @return Die Zeitdifferenz.
 * @author JoSch
 * @date 09-11-2004
 */
bool operator<(const xtime &a, const unsigned RL_LONGLONG b)
{
    xtime _b;
    _b.sec = a.sec;
    _b.nsec = b;
    return xtime_cmp(a, _b) < 0;
}

/** Vergleiche die Zeit a und b.
 * @param a
 * @param b
 * @return Die Zeitdifferenz.
 * @author JoSch
 * @date 09-11-2004
 */
bool operator>(const xtime &a, const unsigned RL_LONGLONG b)
{
    xtime _b;
    _b.sec = a.sec;
    _b.nsec = b;
    return xtime_cmp(a, _b) > 0;
}

/** Ermittle die Zeitdifferenz zwischen a und b.
 * @param a
 * @param b
 * @return Die Zeitdifferenz.
 * @author JoSch
 * @date 09-11-2004
 */
signed RL_LONGLONG operator-(const unsigned RL_LONGLONG a, const xtime &b)
{
    xtime _a;
    _a.sec = b.sec;
    _a.nsec = a;
    return xtime_cmp(_a, b);
}

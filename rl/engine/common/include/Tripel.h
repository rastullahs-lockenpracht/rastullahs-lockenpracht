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

#ifndef __RL_TRIPEL_H__
#define __RL_TRIPEL_H__

namespace rl
{
    template <typename T> class Tripel
    {
    public:
        T first;
        T second;
        T third;

        Tripel()
            : first()
            , second()
            , third()
        {
        }

        Tripel(const T& t1, const T& t2, const T& t3)
            : first(t1)
            , second(t2)
            , third(t3)
        {
        }

        Tripel(const Tripel& rhs)
            : first(rhs.first)
            , second(rhs.second)
            , third(rhs.third)
        {
        }

        Tripel& operator=(const Tripel& rhs)
        {
            if (this != &rhs)
            {
                first = rhs.first;
                second = rhs.second;
                third = rhs.third;
            }
            return *this;
        }

        bool operator==(const Tripel& rhs)
        {
            return first == rhs.first && second == rhs.second && third == rhs.third;
        }

        bool operator<(const Tripel& rhs)
        {
            return first < rhs.first || (first == rhs.first && second < rhs.second)
                || (first == rhs.first && second == rhs.second && third < rhs.third);
        }
    };
}

#endif

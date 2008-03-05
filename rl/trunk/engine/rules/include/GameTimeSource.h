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

#ifndef __GameTimeSource_H__
#define __GameTimeSource_H__

#include "TimeSource.h"

#include "Date.h"

namespace rl
{

    class GameTimeSource : public RealTimeInterruptable
    {
    public:
        GameTimeSource();
        ~GameTimeSource();

        Date getDate() const;
        void setDate(const Date& date);

        virtual TimeSourceType getType() const;
        virtual const Ogre::String& getName() const;
    };

}

#endif // __GameTimeSource_H__

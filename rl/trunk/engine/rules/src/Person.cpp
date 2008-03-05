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
#include "stdinc.h" //precompiled header

#include "Person.h"

namespace rl {

    const Ogre::String Person::CLASS_NAME = "Person";

	Person::Person(unsigned int id)
    	: Creature(id)
    {
        mQueryFlags |= QUERYFLAG_PERSON;
		setWert(WERT_SOZIALSTATUS, 0);
        setWert(WERT_KAMPFUNFAEHIGKEITSSCHWELLE, 5);
    }

    CeGuiString Person::getRasse()
    {
        return mRasse;
    }

    void Person::setRasse(CeGuiString rasse)
    {
        mRasse = rasse;
    }

    CeGuiString Person::getKultur()
    {
        return mKultur;
    }

    void Person::setKultur(CeGuiString kultur)
    {
        mKultur = kultur;
    }

    CeGuiString Person::getProfession()
    {
        return mProfession;
    }

    void Person::setProfession(CeGuiString profession)
    {
        mProfession = profession;
    }

}

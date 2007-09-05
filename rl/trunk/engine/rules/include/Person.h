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

#ifndef __Person_h__
#define __Person_h__

#include "RulesPrerequisites.h"

#include "Creature.h"

namespace rl {

	class _RlRulesExport Person : public Creature
	{
	public:
        static const Ogre::String CLASS_NAME;

        Person(unsigned int id);

        CeGuiString getRasse();
        void setRasse(CeGuiString rasse);
        CeGuiString getKultur();
        void setKultur(CeGuiString kultur);
        CeGuiString getProfession();
        void setProfession(CeGuiString profession);

    private:
        CeGuiString mRasse;
        CeGuiString mKultur;
        CeGuiString mProfession;
	};	
}


#endif

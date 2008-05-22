/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __GameTask_H__
#define __GameTask_H__

#include "CommonPrerequisites.h"

namespace rl {
/** Diese Klasse ist die Basisklasse aller Synchronisierten Aufgaben, die jeden Frame ausgeführt werden.
	Dazu gehören zum Beispiel Animationen.
*/
class _RlCommonExport GameTask
{
    public:
		/// Default Konstruktor, startet unpausiert
        GameTask();
		/// Virtueller Basis-Destruktor
        virtual ~GameTask( ) {};

		/** Wird vom Gameloop aufgerufen, wenn nicht pausiert, 
            wird von  Unterklassen implementiert

			@param elapsedTime Die vergangene Zeit
		*/
		virtual void run( Ogre::Real elapsedTime ) = 0;
        
		/// Gibt zurück ob dieser GameTask pausiert ist
        bool isPaused() const;
		/// Pausiert/Unpausiert den GameTask
        void setPaused( bool isPaused );

    private:
		/// Pause
        bool mPaused;
};

}
#endif

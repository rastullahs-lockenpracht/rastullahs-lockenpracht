/* SoundUpdateTask.cpp - Diese Klassse updatet die 3D-Informationen
 *      von FMOD.
 * (C) 2005. Team Pantheon. www.team-pantheon.de
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

#ifndef _SOUNDUPDATETASK_H_
#define _SOUNDUPDATETASK_H_

#include "MultimediaPrerequisites.h"
#include "GameTask.h"

namespace rl {
    /**
     * Diese Klasse wird in GameLoop registriert
     * und erlaubt es damit regelmässig die FMOD-
     * 3D-Informationen zu aktualisieren.
     * @author JoSch
     * @date 07-24-05
     * @version 1.0
     */
    class _RlMultimediaExport SoundUpdateTask: public GameTask,
        protected Ogre::Singleton<SoundUpdateTask>
    {
        public:
            /// Singleton
            static SoundUpdateTask & getSingleton(void);
            /// Singleton
            static SoundUpdateTask * getSingletonPtr(void);
            /// Die eigentliche Aktion
            virtual void run( Ogre::Real elapsedTime );
            
    };

} // End of namespace rl
#endif //_SOUNDUPDATETASK_H_

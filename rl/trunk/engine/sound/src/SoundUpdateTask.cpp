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
#include "SoundUpdateTask.h"
#include "SoundSubsystem.h"

template<> rl::SoundUpdateTask* Ogre::Singleton<rl::SoundUpdateTask>::ms_Singleton = 0;

using namespace Ogre;

namespace rl {
    
    /** 
     * @param
     * @author JoSch
     * @date 07-24-05
     */
    void SoundUpdateTask::run(Real elapsedTime)
    {
        if (SoundSubsystem::getSingletonPtr())
        {
            SoundSubsystem::getSingleton().update(elapsedTime);
        }
    }

	/**
	* @return Das Singleton-Objekt
	* @author JoSch
	* @date 05-26-2004
	*/
	SoundUpdateTask& SoundUpdateTask::getSingleton(void)
	{
		return Singleton<SoundUpdateTask>::getSingleton();
	}

	/**
	* @return Das Singleton-Zeiger
	* @author JoSch
	* @date 05-26-2004
	*/
	SoundUpdateTask* SoundUpdateTask::getSingletonPtr(void)
	{
		return Singleton<SoundUpdateTask>::getSingletonPtr();
	}
    
} // End of namespace rl

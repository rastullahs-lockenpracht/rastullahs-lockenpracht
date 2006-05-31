/* SoundUpdateTask.cpp - Diese Klassse updatet die 3D-Informationen.
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

#include "SoundManager.h"

using namespace Ogre;

namespace rl {

	SoundUpdateTask::SoundUpdateTask(rl::SoundManager *manager)
		: mSoundManager(manager)
	{
	}
    
    /** 
     * @param
     * @author JoSch
     * @date 07-24-05
     */
    void SoundUpdateTask::run(Real elapsedTime)
    {
        mSoundManager->update();
    }

    
} // End of namespace rl

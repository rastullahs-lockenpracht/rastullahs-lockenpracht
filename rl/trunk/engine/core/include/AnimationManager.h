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

#ifndef __AnimationManager_H__
#define __AnimationManager_H__

#include <list>
#include <OgreSingleton.h>

#include "SynchronizedTask.h"

#include "CorePrerequisites.h"

namespace rl {

///@todo Skalierung der Abspielgeschwindigkeit ermoeglichen.
class _RlCoreExport AnimationManager : public SynchronizedTask, protected Singleton<AnimationManager>
{
public:
    /** Default Constructor */
	AnimationManager( );
	/** Default Deconstructor */
    virtual ~AnimationManager();

    void addAnimation(AnimationState* newAnimState);
    void removeAnimation(AnimationState* oldAnimState);

    virtual void run(Real timePassed);

    static AnimationManager & getSingleton(void);
	static AnimationManager * getSingletonPtr(void);
private:
    std::list<AnimationState*> mAnimationList;
};
}
#endif

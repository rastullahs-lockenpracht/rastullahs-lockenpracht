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

#ifndef __ParticleSystemActor_H__
#define __ParticleSystemActor_H__

#include "CorePrerequisites.h"
#include "Actor.h"

namespace rl {

class _RlCoreExport ParticleSystemActor : public virtual Actor
{
public:
    /** Constructor
		@param name The Actors name
		@param pSceneNode The SceneNode of the ParticleSystem
		@param pParticleSystem The ParticleSystem
	*/
	ParticleSystemActor( const String& name, ParticleSystem* pParticleSystem );

	/** Constructor
		@param name The Actors name
		@param pParentSceneNode The Parent SceneNode for the ParticleSystem
		@param pParticleSystem The ParticleSystem
	*/
	ParticleSystemActor( const String& name, SceneNode* pParentSceneNode, ParticleSystem* pParticleSystem );
    
    /** Default Deconstructor */
	virtual ~ParticleSystemActor();

    void setScale( Real x, Real y, Real z );

	/** Returns the Typename */
	virtual const String& getTypeName();

	/** Gets the internal ParticleSystem object. */
	ParticleSystem* getOgreParticleSystem(void)
	{
		return mParticleSystem;
	}
private:
    static const String TYPENAME;
    ParticleSystem* mParticleSystem;

};

}

#endif

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

#ifndef __GameActor_H__
#define __GameActor_H__

#include "CorePrerequisites.h"

#include "MeshActor.h"
#include "AudibleActor.h"
//#include "ScriptObject.h"
#include "ListenerActor.h"

namespace rl {

class _RlCoreExport GameActor 
	:	public MeshActor,
		public AudibleActor
		//public ScriptObject,
{
public:
    
	/** Constructs a GameActor 
		@param name The Actors unique name
	*/
	GameActor( const String& name, Entity* pEntity);
	GameActor( const String& name, SceneNode* pParentSceneNode, Entity* pEntity);

	virtual ~GameActor();

	void playSound(const String &name);

	/** Returns the TypeName */
	virtual const String& getTypeName();

	static GameActor* castDown( Actor* pActor )
	{	return dynamic_cast<GameActor*>(pActor);  }

    /// Sets the position of this object. 
    virtual void setPosition(Real x, Real y, Real z);
    /// Sets the orientation of this object.
    virtual void setOrientation(const Quaternion& orientation);

    virtual bool isSimpleOgreActor();
protected:
	/** The TypeName */
	static const String TYPENAME;
};

}
#endif

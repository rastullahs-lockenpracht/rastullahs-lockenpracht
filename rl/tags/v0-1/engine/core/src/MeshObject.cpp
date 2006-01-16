/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "MeshObject.h"
#include "Actor.h"
#include "CoreSubsystem.h"
#include "World.h"

#include "Animation.h"
#include "AnimationManager.h"

#include <OgreMeshManager.h>
using namespace Ogre;

namespace rl {
    MeshObject::MeshObject(const String& name, const String& meshname)
    {
		if (meshname.length() > 0)
		{
			mMovableObject = CoreSubsystem::getSingletonPtr()->getWorld()
				->getSceneManager()->createEntity(name, meshname);
			calculateSize();
		}
    }

	MeshObject::~MeshObject()
	{
        stopAllAnimations();
		CoreSubsystem::getSingletonPtr()->getWorld()
			->getSceneManager()->destroyEntity( getEntity() );		
	}

    Entity* MeshObject::getEntity() const
    {
        return reinterpret_cast<Entity*>(mMovableObject);
    }

    ///@todo Sehr, sehr viel Optmimierungsspielraum :)
    ///@todo Padding aus MeshManager::getBoundsPaddingFactor() benutzen
    Vector3 MeshObject::getSize()
    {
        return mSize;
    }
    
    Vector3 MeshObject::getCenter()
    {
        return Vector3(0, 0, getHeight() / 2.0);
    }

    Real MeshObject::getRadius()
    {
        Vector3 extent = getSize();
        return std::max(extent.x, extent.z) / 2.0;
    }

    Real MeshObject::getHeight()
    {
        return getSize().y;
    }

	/// @todo Exception Handling

	Animation* MeshObject::getAnimation(const String& animName) const
	{
		try
		{
			AnimationState* animState = getEntity()->getAnimationState(animName);
			return AnimationManager::getSingleton().getAnimation(animState);
		}
		catch(Ogre::Exception&) 
		{
		}

		return 0;
	}


	Animation* MeshObject::startAnimation(const String& animName, Real speed, unsigned int timesToPlay )
	{
		try
		{
			AnimationState* animState = getEntity()->getAnimationState(animName);
			return AnimationManager::getSingleton().addAnimation(animState,this,speed,timesToPlay);
		}
		catch(Ogre::Exception&) 
		{
		}

		return 0;
	}

	Animation* MeshObject::replaceAnimation(const Ogre::String& oldAnimName, const Ogre::String& newAnimName, Real speed, unsigned int timesToPlay )
	{
        stopAnimation(oldAnimName);
        return startAnimation( newAnimName, speed, timesToPlay );
	}

	void MeshObject::stopAnimation(const Ogre::String& animName)
	{
		try
		{
			AnimationState* animState = getEntity()->getAnimationState(animName);
			AnimationManager::getSingleton().removeAnimation(animState);
		}
		catch(Ogre::Exception&) 
		{ 
		}
	}
    
	void MeshObject::stopAllAnimations( )
	{
			AnimationStateSet* animStates = getEntity()->getAllAnimationStates();
			if (animStates != NULL)
			{
				AnimationStateIterator iter = animStates->getAnimationStateIterator();

				while(iter.hasMoreElements()) 
				{
					AnimationState* state = iter.getNext(); 
					stopAnimation( state->getAnimationName() );
				} 
			}
	}


    String MeshObject::getObjectType()
    {
        return "MeshObject";
    }
    
    void MeshObject::calculateSize()
    {
        Vector3 size = Vector3::ZERO;

        const AxisAlignedBox& aab = getEntity()->getBoundingBox();
        Vector3 x = aab.getMaximum();
        Vector3 n = aab.getMinimum();
        Vector3 scaler = (x - n) * MeshManager::getSingleton().
            getBoundsPaddingFactor();
        size = ((x - scaler) - (n + scaler));
        mSize = size;
    }
    
    void MeshObject::setCastShadows (bool enabled)
    {
        getEntity()->setCastShadows(enabled);
    }

    bool MeshObject::getCastShadows (void) const
    {
        return getEntity()->getCastShadows();
    }

    void MeshObject::setHighlighted( bool highlight )
    {
        String nameExtension = "_HighLight";
        Entity* ent = getEntity();
        int numEnts = ent->getNumSubEntities();

        for( int i = 0; i < numEnts; i++ )
        {
            SubEntity* subent = ent->getSubEntity(i);

            if( subent == NULL )
                continue;

            // TODO - optimieren, nur wenn der Typ verändert wird
            //if(StringUtil::endsWith(subent->getMaterialName(),nameExtension)
            // == highlight )
            //  continue;  

            MaterialPtr oldMaterial = subent->getMaterial();

            // Highlight setzen
            if( highlight )
            {           	 
                MaterialPtr material = MaterialManager::getSingleton().getByName( oldMaterial->getName()+nameExtension );

                if( material.isNull() )
                {
                    material = oldMaterial->clone( oldMaterial->getName()+nameExtension );

                    material->setAmbient(1.0, 1.0, 1.0);
                    material->setDiffuse(1.0, 1.0, 1.0, 1.0);
                    material->setSelfIllumination(0.4, 0.4, 0.4); 
                }

                subent->setMaterialName(material->getName());
            }
            // Highlight entfernen
            else
            {
                String matName = oldMaterial->getName();
                matName = matName.erase(matName.length() - nameExtension.length(), nameExtension.length() );
                subent->setMaterialName( matName );				
            }


        }
    }

	bool MeshObject::isMeshObject()
	{
		return true;
	}
}

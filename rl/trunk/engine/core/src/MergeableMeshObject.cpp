/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#include "MergeableMeshObject.h"

#include <MeshMergeTool.h>
#include <OgreMeshManager.h>
#include <OgreNode.h>
#include <OgreSceneNode.h>
#include <OgreSkeletonManager.h>
#include <OgreTagPoint.h>

#include "CoreSubsystem.h"
#include "World.h"

using namespace Ogre;
using meshmagick::MeshMergeTool;

namespace rl
{
	MergeableMeshObject::MergeableMeshObject(const Ogre::String& name, const Ogre::String& meshname)
		: MeshObject(name, meshname),
		mMeshes(),
		mCombinedMeshName()
	{
		mBaseMesh = getEntity()->getMesh();
	}

	MergeableMeshObject::~MergeableMeshObject()
	{
	}

	void MergeableMeshObject::addSubmesh(const Ogre::String& partname, const Ogre::String& meshfile)
	{
		mMeshes[partname] = meshfile;
		updateMesh();
	}

	void MergeableMeshObject::updateMesh()
	{
		Ogre::String newMeshName = "";
		for (MeshPartMap::const_iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
		{
			newMeshName += "-" + (*it).second;
		}

		if (mCombinedMeshName != newMeshName)
		{
			MeshPtr newMesh = MeshManager::getSingleton().getByName(newMeshName);
			
			if (newMesh.isNull())
			{
				MeshMergeTool mm;
				for (MeshPartMap::const_iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
				{
					String curMeshfile = (*it).second;
					MeshPtr meshToAdd = MeshManager::getSingleton().load(curMeshfile, "regressiontest"); ///@todo this is just for testing
					mm.addMesh(meshToAdd);
				}
				
				newMesh = mm.bake(newMeshName);
			}

			switchTo(newMesh);

			mCombinedMeshName = newMeshName;
		}
	}

	void MergeableMeshObject::setBaseMeshPart(const Ogre::String& partname)
	{
		mMeshes[partname] = mBaseMesh->getName();
	}

	void MergeableMeshObject::replaceSubmesh(const Ogre::String& partToReplace, const Ogre::String& substituteMeshname)
	{
		///@todo check if submesh exists
		mMeshes[partToReplace] = substituteMeshname;
		updateMesh();
	}

	void MergeableMeshObject::removeSubmesh(const Ogre::String& partToRemove)
	{
		MeshPartMap::iterator it = mMeshes.find(partToRemove);
		if (it != mMeshes.end())
		{
			mMeshes.erase(it);
			updateMesh();
		}
		else
		{
			LOG_ERROR(
				Logger::CORE, 
				"Could not remove part '"
				 + partToRemove + "' from mergeable mesh '"
				 + getMeshName() + "'. Part not found.");
		}
	}

	void MergeableMeshObject::switchTo(const MeshPtr& newMesh)
	{
		///@todo reactivate running animations?
		stopAllAnimations();

		Entity* oldEnt = getEntity();
		Entity* newEnt = CoreSubsystem::getSingletonPtr()->getWorld()
			->getSceneManager()->createEntity(oldEnt->getName()+"_", newMesh->getName());
		
		Node* parentNode = oldEnt->getParentNode();
		if (parentNode != NULL)
		{
			TagPoint* tp = dynamic_cast<TagPoint*>(parentNode);
			if (tp != NULL)
			{
				tp->setChildObject(newEnt);
			}
		}

		SceneNode* parentSceneNode = oldEnt->getParentSceneNode();
		if (parentSceneNode != NULL)
		{
			parentSceneNode->detachObject(oldEnt);
			parentSceneNode->attachObject(newEnt);
		}

		mMovableObject = newEnt;
		CoreSubsystem::getSingletonPtr()->getWorld()
			->getSceneManager()->destroyEntity(oldEnt);
	}
}

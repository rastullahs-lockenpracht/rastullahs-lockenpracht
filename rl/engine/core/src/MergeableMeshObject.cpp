/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#include "stdinc.h" //precompiled header

#include "MergeableMeshObject.h"

#include <MmMeshMergeTool.h>

#include "CoreSubsystem.h"
#include "World.h"

using namespace Ogre;
using meshmagick::MeshMergeTool;

namespace rl
{
    MergeableMeshObject::MergeableMeshObject(const Ogre::String& name, const Ogre::String& meshname)
        : MeshObject(name, meshname)
        , mMeshes()
        , mCombinedMeshName()
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

            if (!newMesh)
            {
                MeshMergeTool mm;
                for (MeshPartMap::const_iterator it = mMeshes.begin(); it != mMeshes.end(); ++it)
                {
                    String curMeshfile = (*it).second;
                    MeshPtr meshToAdd = MeshManager::getSingleton().getByName(curMeshfile);

                    if (meshToAdd)
                    {
                        meshToAdd->load();

                        try
                        {
                            mm.addMesh(meshToAdd);
                        }
                        catch (const std::logic_error& ex)
                        {
                            LOG_ERROR(Logger::CORE, ex.what());
                        }
                    }
                    else
                    {
                        LOG_ERROR(Logger::CORE, "Mesh '" + curMeshfile + "' could not be found for merging.");
                    }
                }
                newMesh = mm.merge(newMeshName);
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
            LOG_ERROR(Logger::CORE,
                "Could not remove part '" + partToRemove + "' from mergeable mesh '" + getMeshName()
                    + "'. Part not found.");
        }
    }

    void MergeableMeshObject::switchTo(const MeshPtr& newMesh)
    {
        ///@todo reactivate running animations?
        stopAllAnimations();

        Entity* oldEnt = getEntity();
        Entity* newEnt = CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->createEntity(
            oldEnt->getName() + "_", newMesh->getName());

        // queryflags uebernehmen
        newEnt->setQueryFlags(oldEnt->getQueryFlags());

        TagPoint* parentTp = dynamic_cast<TagPoint*>(oldEnt->getParentNode());
        if (parentTp)
        {
            parentTp->setChildObject(newEnt);
        }

        SceneNode* parentSceneNode = oldEnt->getParentSceneNode();
        if (parentSceneNode != NULL)
        {
            parentSceneNode->detachObject(oldEnt);
            parentSceneNode->attachObject(newEnt);
        }

        std::map<Ogre::String, Ogre::MovableObject*> childObjectMap;

        for (Entity::ChildObjectListIterator it = oldEnt->getAttachedObjectIterator(); it.hasMoreElements();)
        {
            MovableObject* mo = it.peekNextValue();

            // go upwards to tagpoint (all attached objects must have a tagpoint as parent)
            TagPoint* parent = dynamic_cast<TagPoint*>(mo->getParentNode());

            if (parent)
            {
                // tagpoints are children of a bone
                Bone* bone = dynamic_cast<Bone*>(parent->getParent());
                Ogre::String key = bone->getName();
                childObjectMap[key] = mo;
            }

            it.moveNext();
        }

        for (std::map<Ogre::String, Ogre::MovableObject*>::const_iterator itChild = childObjectMap.begin();
             itChild != childObjectMap.end(); ++itChild)
        {
            oldEnt->detachObjectFromBone(itChild->second);
            newEnt->attachObjectToBone(itChild->first, itChild->second);
        }

        mMovableObject = newEnt;
        CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->destroyEntity(oldEnt);
    }
}

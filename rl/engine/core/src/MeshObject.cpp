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

#include "MeshObject.h"

#include "Actor.h"
#include "AnimationManager.h"
#include "CoreSubsystem.h"
#include "Exception.h"
#include "MeshAnimation.h"
#include "World.h"

using namespace Ogre;

namespace rl
{
    MeshObject::MeshObject(const String& name, const String& meshname)
        : mMeshName()
        , mSize()
        , mPoseSizes()
    {
        if (meshname.length() > 0)
        {
            mMeshName = meshname;
            Entity* entity
                = CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->createEntity(name, meshname);
            if (entity->isHardwareAnimationEnabled())
            {
                for (unsigned int i = 0; i < entity->getNumSubEntities(); ++i)
                {
                    SubEntity* se = entity->getSubEntity(i);
                    se->setCustomParameter(0, Vector4(0, 0, 0, 0));
                }
            }
            mMovableObject = entity;
            mSize = calculateDefaultSize();
        }
    }

    MeshObject::~MeshObject()
    {
        if (getEntity() != NULL)
        {
            stopAllAnimations();
            CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager()->destroyEntity(getEntity());
        }
    }

    Entity* MeshObject::getEntity() const
    {
        return static_cast<Entity*>(mMovableObject);
    }

    AxisAlignedBox MeshObject::getDefaultSize() const
    {
        return mSize;
    }

    AxisAlignedBox MeshObject::getPoseSize(const String& animationName)
    {
        PoseMap::iterator it = mPoseSizes.find(animationName);
        if (it == mPoseSizes.end())
        {
            // Not yet calculated. Do so now and save.
            // Duplicating the MeshObject, the restrictions of
            // calculateSize (animation not possible) don't matter any more
            MeshObject* tempMesh = createPosedCopy(animationName);
            AxisAlignedBox rval = tempMesh->calculateSize();
            mPoseSizes.insert(make_pair(animationName, rval));

            delete tempMesh;

            return rval;
        }
        else
        {
            return it->second;
        }
    }

    const Ogre::String& MeshObject::getMeshName() const
    {
        return mMeshName;
    }

    bool MeshObject::hasAnimation(const String& animName) const
    {
        AnimationStateSet* animStates = getEntity()->getAllAnimationStates();
        if (!animStates)
        {
            return false;
        }
        return animStates->hasAnimationState(animName);
    }

    MeshAnimation* MeshObject::getAnimation(const String& animName)
    {
        AnimationState* animState = 0;

        try
        {
            animState = getEntity()->getAnimationState(animName);
        }
        catch (Ogre::ItemIdentityException&)
        {
            LOG_WARNING(Logger::CORE, mMeshName + " has no animation " + animName + ".");
            return NULL;
        }
        catch (Ogre::Exception&)
        {
            LOG_ERROR(Logger::CORE, animName + " is not a valid AnimationState of " + mMeshName + ".");
            return NULL;
        }

        MeshAnimation* anim = dynamic_cast<MeshAnimation*>(AnimationManager::getSingleton().getAnimation(animState));
        if (!anim)
        {
            anim = AnimationManager::getSingleton().addMeshAnimation(animState, this, 1.0, 0, true);
        }

        return anim;
    }

    MeshAnimation* MeshObject::startAnimation(const String& animName, Real speed, unsigned int timesToPlay)
    {
        AnimationState* animState = 0;

        try
        {
            animState = getEntity()->getAnimationState(animName);
        }
        catch (Ogre::ItemIdentityException&)
        {
            LOG_WARNING(Logger::CORE, mMeshName + " has no animation " + animName + ".");
            return NULL;
        }
        catch (Ogre::Exception&)
        {
            LOG_ERROR(Logger::CORE, animName + " is not a valid AnimationState of " + mMeshName + ".");
            return NULL;
        }

        return AnimationManager::getSingleton().addMeshAnimation(animState, this, speed, timesToPlay);
    }

    MeshAnimation* MeshObject::replaceAnimation(
        const String& oldAnimName, const String& newAnimName, Real speed, unsigned int timesToPlay)
    {
        stopAnimation(oldAnimName);
        return startAnimation(newAnimName, speed, timesToPlay);
    }

    void MeshObject::stopAnimation(const String& animName)
    {
        AnimationState* animState = 0;
        try
        {
            animState = getEntity()->getAnimationState(animName);
        }
        catch (Ogre::ItemIdentityException&)
        {
            LOG_WARNING(Logger::CORE, mMeshName + " has no animation " + animName + ".");
        }
        catch (Ogre::Exception&)
        {
            LOG_ERROR(Logger::CORE, animName + " is not a valid AnimationState of " + mMeshName + ".");
        }
        AnimationManager::getSingleton().removeAnimation(animState);
    }

    void MeshObject::stopAllAnimations()
    {
        if (getEntity() == NULL) // Entity schon abgebaut?
        {
            return;
        }
        AnimationStateSet* animStates = getEntity()->getAllAnimationStates();
        if (animStates != NULL)
        {
            AnimationStateIterator iter = animStates->getAnimationStateIterator();

            while (iter.hasMoreElements())
            {
                AnimationState* state = iter.getNext();
                stopAnimation(state->getAnimationName());
            }
        }
    }

    void MeshObject::stopAllAnimationsExcept(const Ogre::String& animName)
    {
        if (getEntity() == NULL) // Entity schon abgebaut?
        {
            return;
        }
        AnimationStateSet* animStates = getEntity()->getAllAnimationStates();
        if (animStates != NULL)
        {
            AnimationStateIterator iter = animStates->getAnimationStateIterator();

            while (iter.hasMoreElements())
            {
                AnimationState* state = iter.getNext();
                if (state->getAnimationName() != animName)
                {
                    stopAnimation(state->getAnimationName());
                }
            }
        }
    }

    String MeshObject::getObjectType() const
    {
        return "MeshObject";
    }

    bool MeshObject::getBlendCumulative(void) const
    {
        Entity* ent = getEntity();
        return ent->hasSkeleton() && (ent->getSkeleton()->getBlendMode() == ANIMBLEND_CUMULATIVE);
    }

    void MeshObject::setBlendCumulative(bool cumulative)
    {
        if (getEntity()->hasSkeleton())
        {
            if (cumulative)
                getEntity()->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);
            else
                getEntity()->getSkeleton()->setBlendMode(ANIMBLEND_AVERAGE);
        }
    }

    AxisAlignedBox MeshObject::calculateDefaultSize()
    {
        const AxisAlignedBox& aabb = getEntity()->getBoundingBox();
        Vector3 x = aabb.getMaximum();
        Vector3 n = aabb.getMinimum();
        Vector3 scaler = (x - n) * MeshManager::getSingleton().getBoundsPaddingFactor();
        AxisAlignedBox rval = AxisAlignedBox(n + scaler, x - scaler);
        return rval;
    }

    MeshObject* MeshObject::createPosedCopy(const String& animName)
    {
        Entity* entity = getEntity();
        MeshObject* newMesh = NULL;
        AnimationStateSet* ass = entity->getAllAnimationStates();

        // Test if wanted anim is available for the entity
        if (!ass || !ass->hasAnimationState(animName))
        {
            Throw(IllegalArgumentException, String("No animation " + animName + " for entity " + entity->getName()));
        }

        // Duplicating the MeshObject, so that the restrictions of animations don't apply
        newMesh = new MeshObject("tempMesh_getPoseSize", getEntity()->getMesh()->getName());
        entity = newMesh->getEntity();
        ass = entity->getAllAnimationStates();

        // Deactivate all anims, save the wanted one.
        AnimationStateIterator it = ass->getAnimationStateIterator();
        while (it.hasMoreElements())
        {
            AnimationState* as = it.peekNextValue();
            if (as->getAnimationName() == animName)
            {
                as->setEnabled(true);
                as->setTimePosition(0.0f);
            }
            else
            {
                as->setEnabled(false);
            }
            it.moveNext();
        }

        // Issue a software skinning request,
        // update the animation and then remove the software request
        // Actually this doesn't work, if animation has been updated already in this frame,
        // but this restriction can't be circumvented easily.
        // This can be circumvented by duplicating the MeshObject, this is done in getPoseSize
        entity->addSoftwareAnimationRequest(false);
        entity->_updateAnimation();

        return newMesh;
    }

    AxisAlignedBox MeshObject::calculateSize()
    {
        Entity* entity = getEntity();
        AxisAlignedBox aabb;

        if (entity->getMesh()->sharedVertexData)
        {
            aabb.merge(getAabbFromVertexData(entity->_getSkelAnimVertexData()));
        }
        for (unsigned int i = 0; i < entity->getNumSubEntities(); ++i)
        {
            SubEntity* se = entity->getSubEntity(i);
            if (se->getSubMesh()->vertexData)
            {
                aabb.merge(getAabbFromVertexData(se->_getSkelAnimVertexData()));
            }
        }

        return aabb;
    }

    AxisAlignedBox MeshObject::getAabbFromVertexData(VertexData* vd)
    {
        AxisAlignedBox aabb;

        const VertexElement* ve = vd->vertexDeclaration->findElementBySemantic(VES_POSITION);
        HardwareVertexBufferSharedPtr vb = vd->vertexBufferBinding->getBuffer(ve->getSource());

        unsigned char* data = static_cast<unsigned char*>(vb->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

        for (size_t i = 0; i < vd->vertexCount; ++i)
        {
            float* v;
            ve->baseVertexPointerToElement(data, &v);
            aabb.merge(Vector3(v[0], v[1], v[2]));

            data += vb->getVertexSize();
        }
        vb->unlock();

        return aabb;
    }

    void MeshObject::setCastShadows(bool enabled)
    {
        getEntity()->setCastShadows(enabled);
    }

    bool MeshObject::getCastShadows(void) const
    {
        return getEntity()->getCastShadows();
    }

    void MeshObject::setHighlighted(bool highlight)
    {
        String nameExtension = "_HighLight";
        Entity* ent = getEntity();
        int numEnts = ent->getNumSubEntities();

        for (int i = 0; i < numEnts; i++)
        {
            SubEntity* subent = ent->getSubEntity(i);

            if (subent == NULL)
                continue;

            // TODO - optimieren, nur wenn der Typ veraendert wird
            // if(StringUtil::endsWith(subent->getMaterialName(),nameExtension)
            // == highlight )
            //  continue;

            if (ent->isHardwareAnimationEnabled())
            {
                subent->setCustomParameter(0, highlight ? Vector4(1, 1, 1, 1) : Vector4(0, 0, 0, 0));
            }

            MaterialPtr oldMaterial = subent->getMaterial();

            // Highlight setzen
            if (highlight)
            {
                MaterialPtr material
                    = MaterialManager::getSingleton().getByName(oldMaterial->getName() + nameExtension);

                if (!material)
                {
                    material = oldMaterial->clone(oldMaterial->getName() + nameExtension);

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
                matName = matName.erase(matName.length() - nameExtension.length(), nameExtension.length());
                subent->setMaterialName(matName);
            }
        }
    }

    bool MeshObject::isMeshObject() const
    {
        return true;
    }

    void MeshObject::setMaterial(const Ogre::String& material, const Ogre::String& submesh)
    {
        if (submesh == "")
        {
            ///@todo implement
        }
        else
        {
            SubEntity* subent = getEntity()->getSubEntity(submesh);
            if (subent)
            {
                subent->setMaterialName(material);
            }
        }
    }
}

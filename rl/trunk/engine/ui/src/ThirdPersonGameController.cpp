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

#include "ThirdPersonGameController.h"
#include "CoreSubsystem.h"
#include "World.h"
#include "InputManager.h"
#include "CommandMapper.h"
#include "DebugWindow.h"
#include "Exception.h"
#include "GameActor.h"

#include <OgreSceneManager.h>
#include <OgreAxisAlignedBox.h>
#include <OgreStringConverter.h>

#include "OgreNoMemoryMacros.h"
#include <ode/ode.h>
#include "OgreMemoryMacros.h"

using namespace Ogre;

namespace rl {

    ThirdPersonGameController::ThirdPersonGameController(
        Camera* camera, GameActor* actor)
        : mSceneManager(CoreSubsystem::getSingletonPtr()->
        getWorld()->getSceneManager()),
        mControlNode(0),
        mLookAtNode(0),
        mCamera(camera),
        mActor(actor),
        mMoveScale(0),
        mRotScale(0),
        mMoveSpeed(200.0),
        mRotSpeed(80.0),
        mFallSpeed(0.1),
        mOdeWorld(0),
        mOdeStepper(0),
        mOdeActor(0),
        mOdeCamera(0),
        mOdeLevel(0),
        mCurrentAnimationState(AS_STAND),
        mLastAnimationState(AS_STAND),
        mMaxPitch(Degree(60.0)),
        mMinPitch(-Degree(89.0)),
        mTargetPitch(-Degree(30.0)),
        mMaxDistance(400.0),
        mMinDistance(0.0),
        mTargetDistance(150.0),
        mDesiredDistance(mTargetDistance),
        mViewMode(VM_THIRD_PERSON)
    {
        if (actor == 0 || camera == 0)
        {
            Throw(NullPointerException, "actor und camera duerfen nicht NULL sein.");
        }

        SceneNode* root = mSceneManager->getRootSceneNode();

        mControlNode = root->createChildSceneNode("ControlNode");

        mLookAtNode = mControlNode->createChildSceneNode("LookAtNode");
        mLookAtNode->setOrientation(Quaternion(mTargetPitch, Vector3::UNIT_X));

        mCameraNode = mLookAtNode->createChildSceneNode("CameraNode");
        mCameraNode->attachObject(mCamera);
        mCameraNode->translate(Vector3(0, 0, mDesiredDistance), Node::TS_LOCAL);

        setupCollisionDetection();
        setup();
    }
    //------------------------------------------------------------------------

    /// This method will be refactored away whith the next
    /// PhysicsManager-Update to OgreODE.
    /// Code adopted from monsters OgreODE-Demo
    void ThirdPersonGameController::setupCollisionDetection()
    {
        // Create the ODE world
        mOdeWorld = new OgreOde::World(mSceneManager);

        mOdeWorld->setGravity(Vector3(0, -980.665, 0));
        mOdeWorld->setCFM(10e-5);
        mOdeWorld->setERP(0.8);
        mOdeWorld->setAutoSleep(true);
        mOdeWorld->setContactCorrectionVelocity(10.0);

        // Create something that will step the world automatically
    	mOdeStepper = new OgreOde::ForwardFixedQuickStepper(0.01);
	    mOdeStepper->setAutomatic(OgreOde::Stepper::AutoMode_NotAutomatic,
	        Root::getSingletonPtr());
            
        // Create the ODE Geometry that represents the hero.
        // These are only dummy values, that are updated
        // when the controlled actor is set.
        mOdeActor = new OgreOde::CapsuleGeometry(1, 1);
        mOdeActorRay = new OgreOde::RayGeometry(1.0);
        mOdeCamera = new OgreOde::SphereGeometry(1);
        
        // Create the TriMesh geometry representing the level
        // Get the mesh that the entity uses
        Mesh* mesh = mSceneManager->getEntity("level")->getMesh();
        SceneNode* levelNode = mSceneManager->getSceneNode("level");
        Vector3 levelPos = levelNode->getWorldPosition();
        Vector3 levelScale = levelNode->_getDerivedScale();
        size_t indices_needed = 0,vertices_needed = 0;
        bool added_shared = false;

        // Calculate how many vertices and indices we're going to need
        for(int i = 0;i < mesh->getNumSubMeshes();i++)
        {
            SubMesh* submesh = mesh->getSubMesh(i);

            // We only need to add the shared vertices once
            if(submesh->useSharedVertices)
            {
                if(!added_shared)
                {
                    VertexData* vertex_data = mesh->sharedVertexData;
                    vertices_needed += vertex_data->vertexCount;
                    added_shared = true;
                }
            }
            else
            {
                VertexData* vertex_data = submesh->vertexData;
                vertices_needed += vertex_data->vertexCount;
            }

            // Add the indices
            Ogre::IndexData* index_data = submesh->indexData;
            indices_needed += index_data->indexCount;
        }

        // Allocate space for the vertices and indices
        Vector3* vertices = new Vector3[vertices_needed];
        int* indices = new int[indices_needed];

        size_t current_offset = 0,shared_offset = 0,next_offset = 0,index_offset = 0;
        added_shared = false;

        // Run through the submeshes again, adding the data into the arrays
        for(int i = 0;i < mesh->getNumSubMeshes();i++)
        {
            SubMesh* submesh = mesh->getSubMesh(i);

            Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
            if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
            {
                if(submesh->useSharedVertices)
                {
                    added_shared = true;
                    shared_offset = current_offset;
                }

                const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
                Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
                unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
                Ogre::Real* pReal;

                for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
                {
                    posElem->baseVertexPointerToElement(vertex, &pReal);

                    vertices[current_offset + j].x = ((*pReal++) * levelScale.x) + levelPos.x;
                    vertices[current_offset + j].y = ((*pReal++) * levelScale.y) + levelPos.y;
                    vertices[current_offset + j].z = ((*pReal++) * levelScale.z) + levelPos.z;
                }
                vbuf->unlock();
                next_offset += vertex_data->vertexCount;
            }

            Ogre::IndexData* index_data = submesh->indexData;

            size_t numTris = index_data->indexCount / 3;
            unsigned short* pShort = 0;
            unsigned int* pInt = 0;
            Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
            bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
            if (use32bitindexes) pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            else pShort = static_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            for(size_t k = 0; k < numTris; ++k)
            {
                size_t offset = (submesh->useSharedVertices)?shared_offset:current_offset;

                unsigned int vindex = use32bitindexes? *pInt++ : *pShort++;
                indices[index_offset + 0] = vindex + offset;
                vindex = use32bitindexes? *pInt++ : *pShort++;
                indices[index_offset + 1] = vindex + offset;
                vindex = use32bitindexes? *pInt++ : *pShort++;
                indices[index_offset + 2] = vindex + offset;

                index_offset += 3;
            }
            ibuf->unlock();
            current_offset = next_offset;
        }

        mOdeLevel = new OgreOde::TriangleMeshGeometry(vertices,
            (int)vertices_needed,indices,(int)indices_needed);

        delete[] vertices;
        delete[] indices;
    }
    //------------------------------------------------------------------------

    ThirdPersonGameController::~ThirdPersonGameController()
    {
        delete mOdeCamera;
        delete mOdeActor;
        delete mOdeLevel;
        delete mOdeStepper;
        delete mOdeWorld;
    }
    //------------------------------------------------------------------------

    void ThirdPersonGameController::run(Real elapsedTime)
    {
        Vector3 translation = Vector3::ZERO;
        Real yaw = 0.0;

        calculateScalingFactors(elapsedTime);
        calculateCameraTranslation();
        calculateHeroTranslation(translation, yaw);
        updateAnimationState(translation);

        translation *= mMoveScale;

        // Runterfallen berücksichtigen.
        // Zuerst Fallgeschwindigkeit berechnen
        translation.y = translation.y - mFallSpeed * elapsedTime;
        mFallSpeed = mFallSpeed - mOdeWorld->getGravity().y * elapsedTime;

        translate(translation, Node::TS_LOCAL);

        mLookAtNode->setOrientation(Quaternion(mTargetPitch, Vector3::UNIT_X));
        mLookAtNode->_update(true, false);
        
        mCameraNode->setPosition(0, 0, mTargetDistance);
        mCameraNode->_update(true, false);
        mOdeCamera->setPosition(mCameraNode->getWorldPosition());
        mTargetDistance = mCameraNode->getPosition().z;
        if (fabs(mTargetDistance - mDesiredDistance) < 2.0)
            mTargetDistance = mDesiredDistance;
        else if (mTargetDistance < mDesiredDistance)
            mTargetDistance += 2.0;
        else 
            mTargetDistance -= 2.0;

        mControlNode->yaw(Degree(yaw));

        mOdeActor->collide(mOdeLevel, this);
        mOdeCamera->collide(mOdeLevel, this);

        if (!InputManager::getSingleton().isCeguiActive())
        {
            updatePickedObject();
        }
    }
    //------------------------------------------------------------------------

    bool ThirdPersonGameController::collision(OgreOde::Contact* contact)
    {
        if (contact->getSecondGeometry() == mOdeActor)
        {
            translate(contact->getNormal() * contact->getPenetrationDepth(),
                Node::TS_WORLD);
            mFallSpeed = 0.0;
        }
        else if(contact->getSecondGeometry() == mOdeCamera
            || contact->getFirstGeometry() == mOdeCamera)
        {
            adjustCamera(contact);                        
        }
        else
        {
            mFallSpeed = 0.1;
        }
        return true;
    }
    //------------------------------------------------------------------------
    
    void ThirdPersonGameController::adjustCamera(OgreOde::Contact* contact)
    {
        //mCameraNode->translate(contact->getNormal() * contact->getPenetrationDepth(),
        //        Node::TS_WORLD);
        mCameraNode->translate(
            Vector3(0.0, 0.0, -contact->getPenetrationDepth()),
            Node::TS_LOCAL);
        mOdeCamera->setPosition(mCameraNode->getWorldPosition());
        mCameraNode->_update(true, false);
        mTargetDistance = mCameraNode->getPosition().z;
    }
    //------------------------------------------------------------------------

    void ThirdPersonGameController::calculateScalingFactors(Real timePassed)
    {
        if (timePassed == 0)
        {
            mMoveScale = 0.5;
            mRotScale = 0.1;
        }
        else
        {
            mMoveScale = mMoveSpeed * timePassed;
            mRotScale = mRotSpeed * timePassed;
        }
    }
    //------------------------------------------------------------------------

    void ThirdPersonGameController::calculateCameraTranslation()
    {	
        InputManager* im = InputManager::getSingletonPtr();
        CommandMapper* cmdmap = CommandMapper::getSingletonPtr();
                        
        mDesiredDistance -= im->getMouseRelativeZ() * 0.05;
        mDesiredDistance = mDesiredDistance > mMaxDistance ?
            mMaxDistance : mDesiredDistance;
        mDesiredDistance = mDesiredDistance < mMinDistance ?
            mMinDistance : mDesiredDistance;
            
        mTargetPitch -= Degree(im->getMouseRelativeY() * 0.13);
        mTargetPitch = mTargetPitch > mMaxPitch ?
            mMaxPitch : mTargetPitch;
        mTargetPitch = mTargetPitch < mMinPitch ?
            mMinPitch : mTargetPitch;        
        
    }
    //------------------------------------------------------------------------

    void ThirdPersonGameController::calculateHeroTranslation(Ogre::Vector3& translation,
        Ogre::Real& yaw)
    {
        InputManager* im = InputManager::getSingletonPtr();
        CommandMapper* cmdmap = CommandMapper::getSingletonPtr();

        yaw = -im->getMouseRelativeX() * 0.13;
        
        if (cmdmap->isMovementActive(TURN_LEFT))
            yaw += mRotScale*2;

        if (cmdmap->isMovementActive(TURN_RIGHT))
            yaw -= mRotScale*2;
        
        if (cmdmap->isMovementActive(MOVE_FORWARD))
            translation.z = -mMoveScale;

        if (cmdmap->isMovementActive(MOVE_BACKWARD))
            translation.z = mMoveScale;

        if (cmdmap->isMovementActive(MOVE_RIGHT))
            translation.x = mMoveScale;

        if (cmdmap->isMovementActive(MOVE_LEFT))
            translation.x = -mMoveScale;

        if (im->isKeyDown(KC_P))
            CoreSubsystem::getSingleton().makeScreenshot("rastullah");

        if (im->isKeyDown(KC_NUMPAD0))
            resetCamera();
            
        if (im->isKeyDown(KC_F))
            setViewMode(mViewMode == VM_FIRST_PERSON ?
                VM_THIRD_PERSON : VM_FIRST_PERSON);
            
        if (im->isKeyDown(KC_SPACE) && fabs(mFallSpeed) <= 0.1)
            mFallSpeed = -200;

        if (im->isKeyDown(KC_L))
            mOdeWorld->setShowDebugObjects(!mOdeWorld->getShowDebugObjects());
            
        translation.normalise();
        if (im->isKeyDown(KC_LSHIFT))
            translation *= 2;
    }
    //------------------------------------------------------------------------

    void ThirdPersonGameController::updateAnimationState(const Vector3& translation)
    {
        mCurrentAnimationState =
            translation != Vector3::ZERO ? AS_WALK_FORWARD : AS_STAND;

        if (mCurrentAnimationState != mLastAnimationState)
        {
            if (mCurrentAnimationState == AS_WALK_FORWARD)
            {
                mActor->startAnimation("gehloop");
            }
            else
            {
                mActor->stopAnimation("gehloop");
            }
            mLastAnimationState = mCurrentAnimationState;
        }
    }
    //------------------------------------------------------------------------

    GameActor* ThirdPersonGameController::getControlledActor()
    {
        return mActor;
    }
    //------------------------------------------------------------------------

    void ThirdPersonGameController::setControlledActor(GameActor* actor)
    {
        if (actor == 0)
        {
            Throw(NullPointerException, "actor darf nicht NULL sein.");
        }
        mActor = actor;
        setup();
    }
    //------------------------------------------------------------------------

    Ogre::Camera* ThirdPersonGameController::getCamera()
    {
        return mCamera;
    }
    //------------------------------------------------------------------------

    void ThirdPersonGameController::setup()
    {
        if (mActor != 0)
        {
            ///@todo dafuer sorgen, dass das nicht mehr noetig ist.
            mActor->getSceneNode()->setScale(0.5, 0.5 , 0.5);

            Vector3 extent = mActor->getExtent();
            SceneNode* root = CoreSubsystem::getSingleton().getWorld()->
                getSceneManager()->getRootSceneNode();
            Vector3 pos = mActor->getPosition();

            // ControlNode auf etwa 10% Abstand bezogen auf die Höhe
            // des GameActors bringen.
            pos.y = pos.y + extent.y * 0.9;
            mControlNode->setPosition(pos);
            mControlNode->addChild(mActor->getSceneNode());
            mActor->getSceneNode()->setPosition(Vector3::ZERO);
            mActor->getSceneNode()->translate(
                Vector3(0, -extent.y * 0.9, 0), Node::TS_PARENT);

            // ODE-Collision-Proxy ist eine Capsule
            mOdeActor->setDefinition(mActor->getRadius(),
                mActor->getHeight() - 2*mActor->getRadius());
            mOdeActor->setPosition(ogrePosToOdePos(
                mActor->getSceneNode()->getWorldPosition(),
                mActor->getExtent()));
            mOdeActor->setOrientation(Quaternion(Degree(90), Vector3::UNIT_X));
            
            // Der Strahl wird für die Kontrolle der Fallgeschwindigkeit verwendet.
            mOdeActorRay->setPosition(mOdeActor->getPosition());
            mOdeActorRay->setOrientation(Quaternion(Degree(90), Vector3::UNIT_X));
            mOdeActorRay->setDefinition(Vector3::ZERO, Vector3::UNIT_Y);
            mOdeActorRay->setLength(-mActor->getHeight()/1.9);
        
            mOdeCamera->setPosition(mCameraNode->getWorldPosition());
            mOdeCamera->setRadius(mCamera->getNearClipDistance() * 1.5);
            
        }
    }
    //------------------------------------------------------------------------

    Vector3 ThirdPersonGameController::ogrePosToOdePos(
        const Vector3& pos, const Vector3& extent)
    {
        return Vector3(pos.x, pos.y + extent.y / 2.0, pos.z);
    }
    //------------------------------------------------------------------------

    void ThirdPersonGameController::setCamera(Ogre::Camera* camera)
    {
        if (camera == 0)
        {
            Throw(NullPointerException, "camera darf nicht NULL sein.");
        }
        mCameraNode->detachObject(mCamera);
        mCameraNode->attachObject(camera);
        mCamera = camera;
        mOdeCamera->setPosition(mCameraNode->getWorldPosition());
        mOdeCamera->setRadius(mCamera->getNearClipDistance() * 1.2);
    }
    //------------------------------------------------------------------------

    void ThirdPersonGameController::updatePickedObject() const
    {
        InputManager::getSingleton().updatePickedObject(0.5, 0.5);
    }
    //------------------------------------------------------------------------
    
    void ThirdPersonGameController::translate(const Vector3& translation,
        Node::TransformSpace ts)
    {
        mControlNode->translate(translation, ts);
        mControlNode->_update(true, false);
        
        mOdeActor->setPosition(ogrePosToOdePos(
            mActor->getSceneNode()->getWorldPosition(),
            mActor->getExtent()));
                
        mOdeActorRay->setPosition(mOdeActor->getPosition());
    }
    //------------------------------------------------------------------------
    
    void ThirdPersonGameController::setPosition(const Vector3& position)
    {
        ///@todo implementieren
    }
    //------------------------------------------------------------------------
    
    void ThirdPersonGameController::setViewMode(ViewMode mode)
    {
        mViewMode = mode;
        if (mode == VM_FIRST_PERSON)
        {
            mActor->getSceneNode()->setVisible(false);
            mDesiredDistance = 0.0;
        }
        else
        {
            mActor->getSceneNode()->setVisible(true);
            mDesiredDistance = 150.0;
            resetCamera();
        }
    }
    //------------------------------------------------------------------------
    
    void ThirdPersonGameController::resetCamera()
    {
        if (mViewMode == VM_THIRD_PERSON)
        {
            mTargetPitch = Degree(-30.0);
            mTargetDistance = 0;
        }
    }
    //------------------------------------------------------------------------

}

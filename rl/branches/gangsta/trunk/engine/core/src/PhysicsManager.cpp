/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "PhysicsManager.h"

#include "CoreSubsystem.h"
#include "World.h"

#include "Actor.h"
#include "ActorControlledObject.h"
#include "MeshObject.h"

#include "Exception.h"

using namespace Ogre;

template<> rl::PhysicsManager* Singleton<rl::PhysicsManager>::ms_Singleton = 0;

namespace rl
{
    int PhysicsManager::LEVELGEOMETRY_GROUP = 31;

    PhysicsManager& PhysicsManager::getSingleton(void)
    {
        return Singleton<PhysicsManager>::getSingleton();
    }

    PhysicsManager* PhysicsManager::getSingletonPtr(void)
    {
        return Singleton<PhysicsManager>::getSingletonPtr();
    }

    PhysicsManager::PhysicsManager( )
    :   m_IsEnabled(false),
        m_LevelGeometry()
    {
        m_GaDriver = Ga::GaPtr<Ga::PhysicsDriver>();        
        m_GaWorld = Ga::GaPtr<Ga::World>();
        m_GaCallback = Ga::GaPtr<Ga::CallbackInterface_Ogre>();

        initializePhysicsManager( );
    }

    void PhysicsManager::initializePhysicsManager( )
    {
        // Den ODE Treiber laden
        m_GaDriver = Ga::Manager::getInstance()->loadPhysicsDriver("ODE");

        // Den Ogre spezifischen Callback-Handler erstellen
        m_GaCallback = new Ga::CallbackInterface_Ogre( 
            CoreSubsystem::getSingleton().getWorld()->getSceneManager() );

        // Callbacks anmelden
        m_GaDriver->getCallbackInterface()->setWorldStepListener(m_GaCallback.get());
        m_GaDriver->getCallbackInterface()->setBodyMovementListener(m_GaCallback.get());
        m_GaDriver->getCallbackInterface()->setDebugVisualProvider(m_GaCallback.get());
        m_GaDriver->getCallbackInterface()->setFileAccessProvider(m_GaCallback.get());
        m_GaDriver->getCallbackInterface()->setMessageOutputProvider(m_GaCallback.get());
        m_GaDriver->getCallbackInterface()->setShapeCollisionListener(m_GaCallback.get());
        m_GaDriver->getCallbackInterface()->setUserObjectListener(m_GaCallback.get());

        // Welt erstellen
        m_GaWorld = m_GaDriver->createWorld("rlWorld");

        Ga::ParameterList parameters;
        m_GaWorld->getSupportedParameters(parameters);

        parameters["gravity"] = Ga::GaVec3(0.0,-9.80665,0.0);
        parameters["extents"] = Ga::GaVec3(1000000.0,500000.0,1000000.0);
        parameters["step"] =   (Ga::GaFloat)(1.0 / 60.0); 

        m_GaWorld->initialise(parameters);

        m_GaWorld->getDefaultMaterial()->setParameter("staticFriction",(Ga::GaFloat)60);
        m_GaWorld->getDefaultMaterial()->setParameter("softCFM",(Ga::GaFloat)0.01);

        m_GaWorld->setDebugVisualMode(Ga::DebugVisual::GDV_SHAPES);
    }

    PhysicsManager::~PhysicsManager()
    {
        Ga::Manager::getInstance()->unloadPhysicsDriver(m_GaDriver);
        m_GaCallback.release();
    }

    void PhysicsManager::run( Ogre::Real elapsedTime )
    {
        m_GaWorld->advanceTime(elapsedTime);
    }

    void PhysicsManager::setGravity( Real x, Real y, Real z )
    {
        Ga::ParameterList parameters;
        
        // Gibt es bisher noch nicht...
        // m_GaWorld->setParameter( "gravity", Ga::GaVec3(x,y,z) );              
    }

    const Vector3& PhysicsManager::getGravity() const
    {
        return Vector3::NEGATIVE_UNIT_Y;
    }

    PhysicalThing* PhysicsManager::createPhysicalThing( 
        GeometryTypes geomType, 
        const Ogre::Vector3& size,
        const Ogre::Vector3& offsetPosition,
        const Ogre::Quaternion& offsetOrientation,
        Real density )
    {
        return NULL;
    }

    void PhysicsManager::clearLevelGeometry()
    {
        // Alle bestehenden Levelgeometrien löschen
        GaShapeListIterator iter;
        for( iter = m_LevelGeometry.begin(); iter != m_LevelGeometry.end(); ++iter )
        {
            Ga::GaPtr<Ga::Shape> shape = *iter;
            //m_GaWorld->destroyShape( shape );
        }
        m_LevelGeometry.clear();
    }
    
    void PhysicsManager::createTestObject( const String& meshname )
    {
        Ga::GaPtr<Ga::Shape> shape = 0;
        Ga::GaPtr<Ga::Body> body = 0;
        Ga::ParameterList params;
        
        body = m_GaWorld->createBody("box",meshname);
        body->getSupportedParameters(params);

        params["inertia_tensor"] = Ga::GaVec3(1,1,1);
        params["mass"] = (Ga::GaFloat)1;

        body->initialise(params);
        body->setPosition( Ga::GaVec3( 160.0, 124.0, 160.0 ) );

        // Create a box shape
        shape = m_GaWorld->createShape("box",body);
        shape->getSupportedParameters(params,"box");

        AxisAlignedBox aab =
            static_cast<Entity*>(body->getUserObject().cast<SceneNode>()
            ->getAttachedObject(0))->getBoundingBox();

        Vector3 vec = ( aab.getMaximum() - aab.getMinimum() ) 
            * ( 1 - MeshManager::getSingleton().getBoundsPaddingFactor() ) ; 
        params["size"] = Ga::GaVec3(vec.x,vec.y,vec.z);

        shape->initialise(params);
    }

    void PhysicsManager::addLevelGeometry( Ogre::Entity* ent )
    {
        Matrix4 m = Matrix4::IDENTITY;

        // Wenn die Entity an einem Node befestigt ist, Transformation übernehmen
        if( ent->getParentNode() != NULL )
            m = ent->getParentNode()->_getFullTransform();

        Ga::GaPtr<Ga::Shape> shape = 
            m_GaCallback->shapeFromEntity(m_GaWorld, ent->getName() ,"mesh", NULL, m );

        shape->setGroup(LEVELGEOMETRY_GROUP);
        m_LevelGeometry.push_back(shape);
    }

    void PhysicsManager::setEnabled(bool enabled)
    {
        m_IsEnabled = enabled;
    }

	void PhysicsManager::toggleDebugGeometry()
	{
        if( m_GaWorld->getDebugVisualMode() == Ga::DebugVisual::GDV_NONE )
            m_GaWorld->setDebugVisualMode(Ga::DebugVisual::GDV_SHAPES);
        else if( m_GaWorld->getDebugVisualMode() != Ga::DebugVisual::GDV_NONE ) 
            m_GaWorld->setDebugVisualMode(Ga::DebugVisual::GDV_NONE);
	}
}

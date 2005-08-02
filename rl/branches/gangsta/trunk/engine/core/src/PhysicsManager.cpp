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
        parameters["extents"] = Ga::GaVec3(1000.0,500.0,1000.0);
        parameters["step"] =   (Ga::GaFloat)(1.0 / 60.0); 

        m_GaWorld->initialise(parameters);

        m_GaWorld->getDefaultMaterial()->setParameter("staticFriction",(Ga::GaFloat)60);
        m_GaWorld->getDefaultMaterial()->setParameter("softCFM",(Ga::GaFloat)0.01);
    }

    PhysicsManager::~PhysicsManager()
    {
        Ga::Manager::getInstance()->unloadPhysicsDriver(m_GaDriver);
        m_GaCallback.release();
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
            m_GaWorld->destroyShape( *iter );
        }
        m_LevelGeometry.clear();

        // Test-Ebene einfügen
        Ga::ParameterList params;
        Ga::GaPtr<Ga::Shape> shape = m_GaWorld->createShape("plane");
        shape->getSupportedParameters(params,"plane");

        params["normal"] = Ga::GaVec3(0.0,1.0,0.0);
        params["distance"] = (Ga::GaFloat)0.0;

        shape->initialise(params);
        shape->setGroup(LEVELGEOMETRY_GROUP);
        m_LevelGeometry.push_back(shape);
    }
    
    void PhysicsManager::createTestConnection( Actor* actor )
    {
        ActorControlledObject* actObj = actor->getControlledObject();
        
        if( actObj->isMeshObject() )
        {
            MeshObject* meshObj = dynamic_cast<MeshObject*>( actObj );
            Entity* ent = meshObj->getEntity();
            //Ga::GaPtr<Ga::Body> body = 0;

            m_GaCallback->shapeFromEntity(m_GaWorld, ent->getName() ,"mesh", NULL );
        }
    }

    void PhysicsManager::setEnabled(bool enabled)
    {
        m_IsEnabled = enabled;
    }

	void PhysicsManager::toggleDebugGeometry()
	{
		
	}
}

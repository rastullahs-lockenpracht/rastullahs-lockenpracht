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

#ifndef __PhysicsManager_H__
#define __PhysicsManager_H__

#include <map>
#include <list>
#include <OgreSingleton.h>
#include <OgreOde.h>
#include "SynchronizedTask.h"

#include "CorePrerequisites.h"


namespace rl {

    class PhysicalThing;
    class Actor;
    class World;

    typedef std::set<PhysicalThing*> PhysicalThingSet;

    class _RlCoreExport PhysicsManager : public SynchronizedTask, public OgreOde::CollisionListener, protected Singleton<PhysicsManager>
    {
    public:
        static const int PT_BOX = 0;
        static const int PT_SPHERE = 1;
        static const int PT_CAPSULE = 2;
        static const int PT_MESH = 3;
        PhysicsManager(rl::World* world);
        virtual ~PhysicsManager();

        virtual void run( Real elapsedTime );

        //PhysicalThing* createSpherePhysicalThing(Actor* actor,
        //    Real density, bool noDynamics = false);
        PhysicalThing* createBoxPhysicalThing(const Ogre::Vector3& size,
            Real density);
        //PhysicalThing* createCappedCylinderPhysicalThing(Actor* actor,
        //    Real density, bool noDynamics = false);

        void removeAndDestroyPhysicalThing(PhysicalThing* thing);

        // Spaces for combining non-colliding objects 
        // ( for example the sword and shield attached to the hero )
        void activateGlobalSpace();
        void activatePhysicalThingSpace(PhysicalThing* thing);
        void removePhysicalThingSpace(PhysicalThing* thing);
        void moveToCurrentSpace(PhysicalThing* thing);
        void createSimpleSpace();

        // Global Settings
        void setGravity( Real x, Real y, Real z );
        Vector3 getGravity();
        void setCFM(Real cfm);
        Real getCFM();
        void setERP(Real erp);
        Real getERP();

        OgreOde::Space* getCurrSpace();
        OgreOde::World* getWorld();
        OgreOde::JointGroup* getContactJointGroup();

        void setLevelGeometry(Ogre::SceneNode* levelNode);
        void setEnabled(bool enabled);

        // Singleton Stuff
        static PhysicsManager & getSingleton(void);
        static PhysicsManager * getSingletonPtr(void);

		bool collision(OgreOde::Contact* contact);
    private:
        bool mEnabled;

        PhysicalThingSet mPhysicalThings;

        std::list<OgreOde::Space*> mSimpleSpaces;
        OgreOde::World* mOdeWorld;
        OgreOde::HashTableSpace* mGlobalSpace;
        OgreOde::Space* mCurrSpace;
        OgreOde::Stepper* mOdeStepper;
        OgreOde::Geometry* mOdeLevel;
    };
}

#endif

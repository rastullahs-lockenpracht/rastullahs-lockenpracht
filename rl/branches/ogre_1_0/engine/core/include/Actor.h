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

#ifndef __Actor_H__
#define __Actor_H__

#include "CorePrerequisites.h"
#include <OgreUserDefinedObject.h>

namespace rl {

    class PhysicalThing;
    class ActorControlledObject;

    class _RlCoreExport Actor : public Ogre::UserDefinedObject
    {
    public:
        ///@todo MovableObject abstrahieren.
        Actor(const Ogre::String& name,
            ActorControlledObject* aco = 0,
            PhysicalThing* pt = 0,
            Ogre::UserDefinedObject* go = 0);

        /// Nicht direkt aufrufen,
        /// sondern ActorManager::destroyActor() benutzen.
        ~Actor();

        /// Returns the unique Name of this Actor
        const Ogre::String& getName();
        
        PhysicalThing* getPhysicalThing();
        Ogre::UserDefinedObject* getGameObject();
        void setGameObject(Ogre::UserDefinedObject* uo);
        
        ActorControlledObject* getControlledObject();
        
        void placeIntoScene(
            const Ogre::Vector3& position = Ogre::Vector3::ZERO,
            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);

        void placeIntoScene(
            Ogre::Real px, Ogre::Real py, Ogre::Real pz,
            Ogre::Real ow, Ogre::Real ox, Ogre::Real oy, Ogre::Real oz);
            
        void removeFromScene();

        /// Gets the current position of this object.
        const Ogre::Vector3& getPosition(void);
        
        /// Sets the position of this object.
        void setPosition(const Ogre::Vector3& vec);
        void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);

        /// Gets the current orientation of this object.
        const Ogre::Quaternion& getOrientation(void);

        /// Sets the orientation of this object.
        void setOrientation(const Ogre::Quaternion& orientation);

        /// Moves the object along it's local  axes.
        void translate(const Ogre::Vector3& d, Ogre::Node::TransformSpace ts);

        /// Rotate the object around the local Z-axis.
        void roll(Ogre::Real angleunits);

        /// Rotate the object around the local X-axis.
        void pitch(Ogre::Real angleunits);

        /// Rotate the object around the local Y-axis.
        void yaw(Ogre::Real angleunits);

        /// Rotate the object around an aritrary axis using a Quarternion.
        void rotate(const Ogre::Quaternion& q);
        
        void attach(const Ogre::String& slot, Actor* actor,
            const Ogre::String& childSlot = "SLOT_DEFAULT");
        void detach(Actor* actor);
        
        Ogre::SceneNode* _getSceneNode();
        Ogre::MovableObject* _getMovableObject();
        void _update();
        void _placeIntoScene(Ogre::SceneNode* parent,
            const Ogre::Vector3& position = Ogre::Vector3::ZERO,
            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);
        
        ///@todo Query-Methoden für Childs
        ///@todo Visibility

    protected:
        typedef std::set<Actor*> ChildSet;
        Ogre::String mName;
        PhysicalThing* mPhysicalThing;
        Ogre::UserDefinedObject* mGameObject;
        ActorControlledObject* mActorControlledObject;
        Actor* mParent;
        ChildSet mChilds;
        Ogre::SceneNode* mSceneNode;
        
        virtual void doAttach(const Ogre::String& slot, Actor* actor,
            const Ogre::String& childSlot);
        virtual void doDetach(Actor* actor);
        /// Argmumente wie placeIntoScene
        virtual void placeChildsIntoScene(const Ogre::Vector3& position,
            const Ogre::Quaternion& orientation);
    };
}
#endif

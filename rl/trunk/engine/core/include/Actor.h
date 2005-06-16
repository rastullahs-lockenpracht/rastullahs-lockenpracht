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
        static Ogre::String DEFAULT_SLOT_NAME;

        enum QueryGroupFlags
        {
            QGF_NONE = 0,
            QGF_DEFAULT = 1,
            QGF_PLAYER = 2,
            QGF_ALL = 0xFFFFFFFF
        };

        Actor(const Ogre::String& name,
            ActorControlledObject* aco = NULL,
            PhysicalThing* pt = NULL,
            Ogre::UserDefinedObject* go = NULL);

        /** Dekonstruktor
          * @remark Nicht direkt aufrufen,
          * sondern ActorManager::destroyActor() benutzen.
          */
        ~Actor();

        /// Gibt den eindeutigen Namen des Aktors zurück
        const Ogre::String& getName() const;
        
        
        Ogre::UserDefinedObject* getGameObject() const;
        void setGameObject(Ogre::UserDefinedObject* uo);
        
        PhysicalThing* getPhysicalThing() const;
        void setPhysicalThing( PhysicalThing* pt );

        ActorControlledObject* getControlledObject() const;
        void setControlledObject( ActorControlledObject* act );
        

        void placeIntoScene(
            const Ogre::Vector3& position = Ogre::Vector3::ZERO,
            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
			const std::string& odeBone = "");   
        void placeIntoScene(
            Ogre::Real px, Ogre::Real py, Ogre::Real pz,
			Ogre::Real ow, Ogre::Real ox, Ogre::Real oy, Ogre::Real oz,
			const std::string& odeBone = "");
            
        void removeFromScene();


        /// Gibt die Anfrage-Maske zurück,
        unsigned long getQueryMask() const;
        /// Setzt die Anfrage-Maske
        void setQueryMask( unsigned long mask = 0xFFFFFFFF );

        /// Fügt der Anfrage-Maske ein Flag hinzu
        void addQueryFlag( unsigned long flag  );
        /// Entfernt ein Flag aus der Anfrage-Maske
        void removeQueryFlag( unsigned long flag );



        /// Gibt die aktuelle Position des Aktors relativ zu seinem Parent zurück
        const Ogre::Vector3& getPosition(void) const;
        /// Gibt die aktuelle Position des Aktors relativ zur Welt zurück
        const Ogre::Vector3& getWorldPosition(void) const;

        /// Setzt die Position des Aktors relativ zu seinem Parent
        void setPosition(const Ogre::Vector3& vec);
        /// Setzt die Position des Aktors relativ zu seinem Parent
        void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);

        /// Skaliert den Aktor
        void setScale( Ogre::Real sx, Ogre::Real sy, Ogre::Real sz );

        /// Gibt die Orientierung des Aktors relativ zu seinem Parent zurück
        const Ogre::Quaternion& getOrientation(void) const;
        /// Gibt die Orientierung des Aktors relativ zur Welt zurück
        const Ogre::Quaternion& getWorldOrientation(void) const;

        /// Setzt die Orientierung des Aktors
        void setOrientation(const Ogre::Quaternion& orientation);

        /// Bewegt den Aktor an seiner Achse entlang
        void translate(const Ogre::Vector3& d, Ogre::Node::TransformSpace ts);

        /// Dreht den Aktor um seine Z-Achse
        void roll(Ogre::Real angleunits);
        /// Dreht den Aktor um seine Z-Achse
        void pitch(Ogre::Real angleunits);
        /// Dreht den Aktor um seine Z-Achse
        void yaw(Ogre::Real angleunits);

        /// Dreht den Aktor
        void rotate(const Ogre::Quaternion& q);
      

        /**
         *	Befestigt einen anderen Aktor über einen UnterNode an diesem Aktors.
         *
         * @param actor Der Aktor
         * @param childSlot Der Slot an dem zu befestigenden Aktor, wenn DEFAULT_SLOT_NAME ignoriert
         *                  verursacht zusätzliche Offset/Drehung
         * @param offsetPosition Die zusätzliche Verschiebung
         * @param offsetOrientation Die zusätzliche Drehung
         */
        void attach( 
            Actor* actor,
            const Ogre::String& childSlot = "SLOT_DEFAULT",
            const Ogre::Vector3& offsetPosition=Ogre::Vector3::ZERO,
            const Ogre::Quaternion& offsetOrientation=Ogre::Quaternion::IDENTITY
            );

        /**
         *	Befestigt einen anderen Aktor über einen UnterNode an diesem Aktors.
         *
         * @param actor Der Aktor
         * @param childSlot Der Slot an dem zu befestigenden Aktor, wenn DEFAULT_SLOT_NAME ignoriert,
         *                  verursacht zusätzliche Offset/Drehung
         * @param offsetPosition Die zusätzliche Verschiebung
         * @param offsetAxis Die Achse der zusätzlichen Drehung
         * @param offsetRotation Die zusätzliche Drehung
         */
        void attach( 
            Actor* actor,
            const Ogre::String& childSlot = "SLOT_DEFAULT",            
            const Ogre::Vector3& offsetPosition=Ogre::Vector3::ZERO,
            const Ogre::Vector3& offsetAxis=Ogre::Vector3::UNIT_X,
            const Ogre::Radian& offsetRotation=Ogre::Radian(0) );

        /**
          *	Befestigt einen anderen Aktor an einem SLOT dieses Aktors.
          *
          * @param actor Der Aktor
          * @param slot Der Slot an diesem Aktor, wenn DEFAULT_SLOT_NAME ignoriert
          * @param childSlot Der Slot an dem zu befestigenden Aktor, wenn DEFAULT_SLOT_NAME ignoriert,
          *                  verursacht zusätzliche Offset/Drehung
          * @param offsetPosition Die zusätzliche Verschiebung
          * @param offsetOrientation Die zusätzliche Drehung
          */
        void attachToSlot( 
            Actor* actor,
            const Ogre::String& slot,
            const Ogre::String& childSlot = "SLOT_DEFAULT",
            const Ogre::Vector3& offsetPosition=Ogre::Vector3::ZERO,
			const Ogre::Quaternion& offsetOrientation=Ogre::Quaternion::IDENTITY
			);

        /**
        *	Befestigt einen anderen Aktor an einem SLOT dieses Aktors.
        *
        * @param actor Der Aktor
        * @param slot Der Slot an diesem Aktor, wenn DEFAULT_SLOT_NAME ignoriert
        * @param childSlot Der Slot an dem zu befestigenden Aktor, wenn DEFAULT_SLOT_NAME ignoriert, 
        *                  verursacht zusätzliche Offset/Drehung
        * @param offsetPosition Die zusätzliche Verschiebung
        * @param offsetAxis Die Achse der zusätzlichen Drehung
        * @param offsetRotation Die zusätzliche Drehung
        */
        void attachToSlot( 
            Actor* actor,
            const Ogre::String& slot,
            const Ogre::String& childSlot = "SLOT_DEFAULT",            
			const Ogre::Vector3& offsetPosition=Ogre::Vector3::ZERO,
            const Ogre::Vector3& offsetAxis=Ogre::Vector3::UNIT_X,
            const Ogre::Radian& offsetRotation=Ogre::Radian(0) );

        /// Entfernt einen zuvor befestigten Aktor
        void detach(Actor* actor);
        
        Ogre::SceneNode* _getSceneNode() const;
        Ogre::MovableObject* _getMovableObject() const;
        void _update();
        void _placeIntoScene(Ogre::SceneNode* parent,
            const Ogre::Vector3& position = Ogre::Vector3::ZERO,
			const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
			const std::string& odeBone = "");        

        /// Setzt diesem Aktor ein Highlight
		void setHighlighted(bool highlight);
        /// Gibt zurück ob der Aktor gehighlighted ist
        bool isHighlighted() const;
        
        ///@todo mehr Query-Methoden für Childs
        Actor* getChildByName(const Ogre::String& name ) const;
        ///@todo Visibility
    private:
        typedef std::set<Actor*> ChildSet;

        /// Der einzigartige Name
        Ogre::String mName;
        /// Verknüpfung zur Physikalischen Repräsentation
        PhysicalThing* mPhysicalThing;
        /// Verknüpfung zur Nächsten Schicht
        Ogre::UserDefinedObject* mGameObject;
        /// Verknüpfung zum kontrollierten Objekt
        ActorControlledObject* mActorControlledObject;
        /// Der Parent-Aktor
        Actor* mParent;
        /// Darunterliegende Aktoren
        ChildSet mChilds;
        /// Der SceneNode des Aktors
        Ogre::SceneNode* mSceneNode;
        /// Speichert ob der Aktor zur Zeit leuchtet
        bool mHighlighted;
        /// Ist der Aktor an einem Bone angehangen?
        bool mAttachedToBone;
		
        /**
         *	Kümmert sich um das Durchführen des Befestigens
         *
         * @param actor Der Aktor
         * @param slot Der Slot an diesem Aktor, wenn DEFAULT_SLOT_NAME ignoriert
         * @param childSlot Der Slot an dem zu befestigenden Aktor, wenn DEFAULT_SLOT_NAME ignoriert
         *                  verursacht zusätzliche Offset/Drehung
         * @param offsetPosition Die zusätzliche Verschiebung
         * @param offsetOrientation Die zusätzliche Drehung
         */
        void doAttach(
            Actor* actor,
            const Ogre::String& slot, 
            const Ogre::String& childSlot, 
            const Ogre::Vector3& offsetPosition,
			const Ogre::Quaternion& offsetOrientation);
        /**
         *	Kümmert sich um das Durchführen des Abnehmens
         */
        void doDetach(Actor* actor);

        void placeChildsIntoScene(
            const Ogre::Vector3& position,
            const Ogre::Quaternion& orientation);
    };
}
#endif

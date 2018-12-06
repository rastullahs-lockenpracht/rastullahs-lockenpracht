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


#ifndef __MovingCreature_H__
#define __MovingCreature_H__

#include "RulesPrerequisites.h"
#include "PhysicsController.h"
#include "PhysicsMaterialRaycast.h"
#include "Creature.h"
#include "Actor.h"
#include <map>
#include <MessagePump.h>



namespace rl
{
    class AbstractMovement;
    class CreatureControllerManager;
    class MeshAnimation;

    /** 
     * This class provides an interface to control the movement of a creature.
     * It handles all nessessary things like animations and calculates the movement speed.
     */
    class _RlRulesExport CreatureController :
        public PhysicsController
    {
    public:
        static const Ogre::String PROPERTY_CREATUREID;
        /**
         * This function is not intented to be called directly
         * @retval true, if this CreatureController still needs to be called every frame
         */
        bool run(Ogre::Real elapsedTime);

        /// Newton force and torque callback
        void OnApplyForceAndTorque(PhysicalThing* thing, float timestep);

        /// Newton contact callback called by the movingCreatureManager
        void userProcess(OgreNewt::ContactJoint &contactJoint, Ogre::Real timestep, int threadid);

        Creature* getCreature() {return mCreature;}
        bool refetchCreature();

        /** 
         * the different movements a creature can perform, 
         * there must be an adequate movement-class
         * (derived from AbstractMovement)
         */
        typedef enum
        {
            // these movements can be used
            MT_NONE,
            MT_STEHEN,
            MT_GEHEN,
            MT_JOGGEN,
            MT_LAUFEN,
            MT_RENNEN,
            MT_RUECKWAERTS_GEHEN,
            MT_RUECKWAERTS_JOGGEN,
            MT_SEITWAERTS_GEHEN,
            MT_SCHLEICHEN,
            MT_HOCHSPRUNG,
            MT_WEITSPRUNG,
            MT_ZIELSPRUNG,
            // these movements are only for internal purposes and should not be used directly
            // instead use one of the movements above
            MT_DREHEN = 100,  // if new movements are added, the old numbers can stay the same
            MT_STUFENERKENNUNG,
            MT_FALLEN,
	    MT_LIEGEN
        } MovementType;

        /// The generalization of the place (in the air, on the floor, in the water...)
        typedef enum { 
            AL_AIRBORNE, // the creature is floating/falling in the air
            AL_FLOOR // the creature is standing/lying/moving/... on the floor
        } AbstractLocation;

        /**
         * sets wether the creature is airborne or not
         * this is done by this class itself every frame, so using this method does probably not
         * have the desired effect
         */
        void setAbstractLocation(AbstractLocation type);

        AbstractLocation getAbstractLocation() const {return mAbstractLocation;}


        /**
         * the most important function: sets a new movement or changes the direction or rotation of a movement
         * if this movement isn't possible, the fallback-movement is called.
         * The movement doesn't change until another movement is set, so it is not necessary to call this function 
         * every frame. Only the rotation is resettet to zero every new frame!
         * @param type the id of the movement
         * @param direction This is the movement direction in local space.
		 *                  If the direction isn't possible (see AbstractMovement::isDirectionPossible),
		 *                  only a "part" of the direction is applied.
         * @param rotation like above (in radians)
         * @retval false signifies that the change to this movement was not possible (possibly because the present movement forbade it)
         */
        bool setMovement(MovementType type, Ogre::Vector3 direction, Ogre::Vector3 rotation);


        MovementType getMovementId() const;
        Ogre::Vector3 getDirection() const {return mDirection;}

        /// This function does probably not return the expected value, because the rotation is resetted every frame!
        Ogre::Vector3 getRotation() const {return mRotation;}

        /// This function can return NULL
        AbstractMovement *getMovement() {return mMovement;}

        /// This function can return NULL
        AbstractMovement *getMovementFromId(MovementType id);

        // some methods used by movements
        int getCurrentGS() const;
        Ogre::Real getMaximumSpeed() const;
        Ogre::Vector3 getVelocity() const; // in local axes
        Ogre::Vector3 getOmega() const;
        MeshAnimation *setAnimation(const Ogre::String &name, 
                          Ogre::Real speed = 1, 
                          unsigned int timesToPlay = 0, 
                          const Ogre::String &collisionName = "", // the name of the animation the collision is based on
                          Ogre::Real weight = 1 // the weight of the animation, the weight of the last animation is set to 1-weight
                          );


        /// this method will return the yaw, the creature tries to turn to, if a rotation-movement is activated. if not, it will return the current yaw
        Ogre::Radian getYaw();
        Ogre::Real getLastMovementChange() const { return mLastMovementChange; }
        MovementType getLastMovementType() const { return mLastMovementType; }
        Ogre::Real getAnimationTimePlayed() const; // this is a relative value between 0 and 1

    protected:
        Creature *mCreature;
        int mGameObjectId;
        AbstractLocation mAbstractLocation;
        
        AbstractMovement *mMovement;
        MovementType mLastMovementType;
        Ogre::Real mLastMovementChange;
        Ogre::Vector3 mDirection;
        Ogre::Vector3 mRotation;

        typedef std::map<MovementType, AbstractMovement*> MovementMap;
        MovementMap mMovementMap;

        // in order to copy the contactcallback members correctly and to create and destroy the creature
        friend class CreatureControllerManager;
        /** Constructor.
         * @param creature the creature of the bot/char
         * @param actor the actor of the bot/char
         */
        CreatureController(Creature *character);
        ~CreatureController();

    private:
        // only used in setAnimation
        Ogre::String mLastAnimationName;
        Ogre::String mLastCollisionName;
        Ogre::Real mLastAnimationSpeed;
        Ogre::String mStillWeightedAnimationName;

        // only used in userProcess
        Time mLastFloorContact;
        Time mLastFloorContact2;

        // used to reset the material
        const OgreNewt::MaterialID *mOldMaterialId;

	PhysicsMaterialRaycast mRaycast;

        MessagePump::ScopedConnection mMessageType_GameObjectsLoaded_Handler;
        MessagePump::ScopedConnection mLifeStateChangedHandler;
    };
}
#endif


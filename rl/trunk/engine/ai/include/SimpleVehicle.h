/* This source file is part of Rastullahs Lockenpracht.
 * Copyright(C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __RlAI_SimpleVehicle_H__
#define __RlAI_SimpleVehicle_H__

#include "AiPrerequisites.h"
#include "OpenSteer/AbstractVehicle.h"
#include "OpenSteer/Proximity.h"

namespace rl
{
    class Actor;
    class Creature;
    class CreatureController;

    class _RlAiExport SimpleVehicle : public OpenSteer::AbstractVehicle
    {        
    protected:
        SimpleVehicle();
    public:
        typedef OpenSteer::AbstractTokenForProximityDatabase<OpenSteer::AbstractVehicle*> ProximityToken;
        SimpleVehicle(Creature* creature);
        virtual ~SimpleVehicle();
        
        inline void setCreature(Creature* creature) { mCreature = creature; }
        inline void setProximityToken(ProximityToken* token) { mProximityToken = token; }

        // convenience functions
        CreatureController* getCreatureController() const;
        const Actor* getActor() const;
        
        virtual float getMass(void) const;
        virtual float setMass(float);

        /** 
         * size of bounding sphere, for obstacle avoidance, etc.
         */
        virtual float getRadius(void) const;
        virtual float setRadius(float);

        /**
         * velocity of vehicle
         */
        virtual Ogre::Vector3 getVelocity() const;

        /** 
         * speed of vehicle  (may be faster than taking magnitude of velocity)
         */
        virtual float getSpeed(void) const;
        virtual float setSpeed(float);

        /**
         * predict position of this vehicle at some time in the future
         */
        virtual Vector3 predictFuturePosition(const float predictionTime) const;

  
        /**
         * the maximum steering force this vehicle can apply
         */
        virtual float getMaxForce(void) const;
        virtual float setMaxForce(float);

        /**
         * the maximum speed this vehicle is allowed to move
         */
        virtual float getMaxSpeed(void) const;
        virtual float setMaxSpeed(float);


        float getHeight() const;

		/**
         * Can be used to update the entity, eg. for steering, flocking etc.pp...
         */
		virtual void update(const float currentTime, const float elapsedTime);
    protected:
        Creature* mCreature;
        ProximityToken* mProximityToken;
    };
}

#endif


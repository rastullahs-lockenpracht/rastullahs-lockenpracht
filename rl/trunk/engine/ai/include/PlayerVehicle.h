/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __RlAI_PlayerVehicle_H__
#define __RlAI_PlayerVehicle_H__

#include "SteeringVehicle.h"

namespace rl
{
	class Agent;
	class Creature;
	/** Represents the player as a vehicle.
	 */
	class PlayerVehicle :
		public SteeringVehicle
	{
	public:
		/** Constructor
		 * @param Actor gives the Actor representing the Player
		 */
		PlayerVehicle(Creature* character);
		/** explicit destructor
		 */
		virtual ~PlayerVehicle(void);

		/** Updates the steering of the vehicle.
		 * Update is done by fetching position and orientation from the
		 * attached PhysicalThing object representing the player. Those
		 * are then used to set the state of the OpenSteer Obstacle
		 * accordingly.
		 * @param currentTime const float current time
		 * @param elapsedTime const float elapsed time since last call
		 */
        // same as SteeringVehicle if mMovingCreature is not set
		//virtual void update(const float currentTime, const float elapsedTime);
    protected:
        virtual void createMovingCreature() {}
        virtual void destroyMovingCreature() {}
	};
}

#endif

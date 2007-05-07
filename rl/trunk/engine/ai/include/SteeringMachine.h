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
#ifndef __RlAi_SteeringMachine_H__
#define __RlAi_SteeringMachine_H__

#include "FuzzyStateMachine.h"
//#include "SteeringVehicle.h"

namespace rl
{
	class SteeringVehicle;
	//! typedefinition for ruby interface
	typedef FuzzyState<SteeringVehicle> SteeringBehaviour;
//	typedef FuzzyStateMachine<Agent> SteeringMachine;
	/** SteeringMachine responsible for steering a Vehicle.
	 */
	class SteeringMachine : public FuzzyStateMachine<SteeringVehicle>
	{
	public:
		/** Constructor
		 * sets parent and controller for the FuzzyStateMachine
		 * @param parent gives the parent FuzzyState object
		 * @param controller sets the controller object
		 */
		SteeringMachine(SteeringBehaviour* parent, SteeringVehicle* controller)
			: FuzzyStateMachine<SteeringVehicle>(parent, controller) {}

		/** returns the type 
		 * @returns CeGuiString containing SteeringMachine as a typestring
		 */
		virtual CeGuiString getType() { return "SteeringMachine"; }
	};
	
}
#endif

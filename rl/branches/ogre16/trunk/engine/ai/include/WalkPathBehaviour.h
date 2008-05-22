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
#ifndef __RlAI_WalkPathBehaviour_H__
#define __RlAI_WalkPathBehaviour_H__

#include "AiPrerequisites.h"
#include "AgentManager.h"
#include "OpenSteer/PolylineSegmentedPathwaySingleRadius.h"
//#include "AgentState.h"
//#include "SteeringMachine.h"
//#include "SteeringVehicle.h"
//#include <stack>

namespace rl
{
    class Landmark;
    class LandmarkPath;
    class WayPointGraph;

	/** This behaviour lets an agent walk along a path while avoiding other "passengers"
     * which are known by the AgentManager
	 */
    class _RlAiExport WalkPathBehaviour : public SteeringBehaviour
	{
	public:
		/** Constructor by Creature object.
		 * @param character Creature object
		 */
		WalkPathBehaviour();

		/** explicit virtual destructor
		 */
		virtual ~WalkPathBehaviour(void);

		/** returns a string describing the type of fuzzy state.
		 * function is abstract.
		 * @returns type name string for fuzzy state.
		 */
		virtual CeGuiString getType();
		/** should initialize the fuzzy state.
		 * function is abstract.
		 */
		virtual void init();
		/** should activate the fuzzy state.
		 * function is abstract.
		 */
		virtual void activate();
		/** deactivates fuzzy state.
		 * function is abstract.
		 */
		virtual void deactivate();
		/** tells the fuzzy state to update for the elapsed time.
		 * @param elapsedtime gives the elapsed time as a float
		 */
		virtual void update(const float elapsedTime);

		/** calculates the activation value.
		 * @returns float containing the calculated activation value.
		 */
		virtual float calculateActivation();

        /** Calculates the path to a Landmark by applying the AStar search on
         * the given WayPointGraph. 
         * The Path is used during the update-calls of the behaviour
         */
        void calcPathToLandmark(Landmark* lm, const WayPointGraph* wps);

        void setLandmarkPath(LandmarkPath* lmp);
    private:
        Landmark* mLandmark;
        OpenSteer::PolylineSegmentedPathwaySingleRadius mPathway;
        int mPathDirection;
	};
}
#endif

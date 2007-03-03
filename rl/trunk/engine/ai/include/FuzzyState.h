/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __RlAI_FuzzyState_H__
#define __RlAI_FuzzyState_H__

#include "AiPrerequisites.h"

namespace rl
{
	/* Represents a single fuzzy state for the FuzzyStateMachine
	 */
	template <class Controller> class FuzzyState
	{
	public:
		/** default constructor
		 * initializes internal variables only
		 */
		FuzzyState(void)
			: mActivationLevel(), mParent(NULL), mController(NULL) {}

		/** Constructor assigning parent FuzzyState and controller object.
		 * The Controller object is normally influenced by FuzzyState
		 * @param parent gives the parent FuzzyState object
		 * @param controller sets the controller object
		 */
		FuzzyState<Controller>(FuzzyState* parent, Controller* controller)
			: mActivationLevel(), mParent(parent), mController(controller) {}

		/** explicit virtual destructor
		 */
		virtual ~FuzzyState(void){};

		/** returns a string describing the type of fuzzy state.
		 * function is abstract.
		 * @returns type name string for fuzzy state.
		 */
		virtual CeGuiString getType()=0;
		/** should initialize the fuzzy state.
		 * function is abstract.
		 */
		virtual void init()=0;
		/** should activate the fuzzy state.
		 * function is abstract.
		 */
		virtual void activate()=0;
		/** deactivates fuzzy state.
		 * function is abstract.
		 */
		virtual void deactivate()=0;
		/** tells the fuzzy state to update for the elapsed time.
		 * @param elapsedtime gives the elapsed time as a float
		 */
		virtual void update(const float elapsedTime)=0;

		/** calculates the activation value.
		 * @returns float containing the calculated activation value.
		 */
		virtual float calculateActivation()=0;

		/** checks if the activation value exceeds the boundary constraints.
		 * If this happens, the value is reset to an according boundary value.
		 */
		virtual void checkBounds(float lowerBound = 0.0f, float upperBound = 1.0f)
		{
			mActivationLevel = std::max(mActivationLevel, lowerBound);
			mActivationLevel = std::min(mActivationLevel, upperBound);
		}

		/** retrieve the current activation level.
		 * @returns float the current activation level.
		 */
		float getActivationLevel() { return mActivationLevel; }
		/** sets the current activation level.
		 * @param activationLevel the new activation level.
		 */
		void setActivationLevel(float activationLevel)  { mActivationLevel = activationLevel; }

		/** retrieve the controller object
		 * @returns the stored controller
		 */
		Controller* getController() { return mController; }
		/** sets the controller object
		 * @param controller new controller pointer
		 */
		void setController(Controller* controller) { mController = controller; }

		/** retrieve the parent fuzzy state
		 * @returns FuzzyState the parent of the fuzzy state
		 */
		FuzzyState* getParent() { return mParent; }
		/** sets the parent fuzzy state
		 * @param parent of the fuzzy state
		 */
		void setParent(FuzzyState* parent) { mParent = parent; }
		
	protected:
		//! stores the current activation level
		float mActivationLevel;
		//! stores the parent fuzzy state
		FuzzyState* mParent;
		//! stores the controller assigned to the fuzzy state
		Controller* mController;
	};
}
#endif

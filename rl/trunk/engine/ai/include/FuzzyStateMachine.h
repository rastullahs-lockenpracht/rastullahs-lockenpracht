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
#ifndef __RlAI_FuzzyStateMachine_H__
#define __RlAI_FuzzyStateMachine_H__
#include <list>
#include "AiPrerequisites.h"
#include "FuzzyState.h"
#include "ScriptWrapper.h"

namespace rl
{
	/** manages a set of FuzzyState objects 
	 */
	template <class Controller> class FuzzyStateMachine :
		public FuzzyState<Controller>
	{
	public:
		//! typedefinition for pointer to a FuzzyState
		typedef FuzzyState<Controller>* FuzzyStatePtr;

		/** Constructor
		 * sets parent and controller for the FuzzyStateMachine
		 * @param parent gives the parent FuzzyState object
		 * @param controller sets the controller object
		 */
		FuzzyStateMachine(FuzzyStatePtr parent, Controller* controller)
			: FuzzyState<Controller>(parent, controller), mMaxActiveStates(), mStates() {}

		/** destructor.
		 * removes internally stored Fuzzystate objects
		 */
		virtual ~FuzzyStateMachine(void)
		{
			clearStates();
		}

		/** adds a FuzzyState object
		 * @param state a FuzzyState object to add.
		 */
		virtual void addState(FuzzyStatePtr state)
		{
			mStates.push_back(State(false, state));
            ScriptWrapper::getSingleton().owned( state );
		}
		/** removes all stored FuzzyState objects
		 */
        virtual void clearStates()
		{
			typename StateList::iterator itr = mStates.begin();
			for(; itr != mStates.begin(); ++itr)
			{
                ScriptWrapper::getSingleton().disowned( itr->second );
				delete itr->second;
			}
			mStates.clear();
		}

		/** Tests if the FuzzyStateMachine is an active FuzzyState object.
		 * So always return false (no).
		 * @returns false
		 */
		virtual bool isActive(FuzzyStatePtr state)
		{
			return false; 
		}
		/** reset function for resetting the FuzzyStateMachine
		 */
		virtual void reset(){}

		/** sets the maximum number of concurrently active states.
		 * @param count maximum number of concurrently active states.
		 */
		virtual void setMaxActivity(int count){ mMaxActiveStates = count; }
		/** fetches the maximum number of concurrently active states.
		 * @param int returns the maximum number of concurrently active states.
		 */
		virtual int getMaxActivity() { return mMaxActiveStates; }

		/** calculates the activation value.
		 * @returns float containing the calculated activation value.
		 */
		float calculateActivation(void){ return 0.0f; }

		/** initializes the fuzzy state.
		 */
		virtual void init(){}
		/** should activate the fuzzy state.
		 */
		virtual void activate(){}
		/** deactivates fuzzy state.
		 */
		virtual void deactivate(){}
		/** tells the fuzzy state machine to update for the elapsed time.
		 * For each stored FuzzyState the activation is calculated and
		 * testified whether it activates the state or not. When a state
		 * is active its update function is called with the elapsed time.
		 * @param elapsedtime gives the elapsed time as a float
		 */
		virtual void update(const float elapsedTime)
		{
			if(mStates.empty())
				return;

			typename StateList::iterator itr = mStates.begin();
			for(; itr != mStates.end(); ++itr)
			{
				bool isActive = (itr->second->calculateActivation() > 0);
				if(isActive)
				{
				//  if state is already active, update it, otherwise activate it
					if(itr->first)
					{
						itr->second->update(elapsedTime);
					}
					else
					{
						itr->second->activate();
						itr->first = true;
						// maybe call update here, too
					}
				}
				else
				{
				//	if state was active, deactivate it, otherwise do nothing
					if(itr->first)
					{
						itr->second->deactivate();
						itr->first = false;
					}	
				}
			}
		}


	protected:
		//! maximum number of active states
		int mMaxActiveStates;
		/** typedefinition for active/inactive and FuzzyState objects.
		 * When the boolean is true, the state is active, otherwise not
		 */
		typedef std::pair<bool, FuzzyStatePtr> State;
		//! defines a double linked list of states
		typedef std::list<State> StateList;
		//! the double linked list of stored FuzzyState objects
		StateList mStates;
	};

/*	template <class Controller> void FuzzyStateMachine<Controller>::update(float elapsedTime)
	{

	}
*/
}
#endif

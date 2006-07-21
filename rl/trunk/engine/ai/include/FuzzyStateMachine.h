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
#ifndef __RlAI_FuzzyStateMachine_H__
#define __RlAI_FuzzyStateMachine_H__
#include <list>
#include "AiPrerequisites.h"
#include "FuzzyState.h"

namespace rl
{
	template <class Controller> class FuzzyStateMachine :
		public FuzzyState<Controller>
	{
	public:
		typedef FuzzyState<Controller>* FuzzyStatePtr;
		FuzzyStateMachine(FuzzyStatePtr parent, Controller* controller)
			: FuzzyState<Controller>(parent, controller), mMaxActiveStates(), mStates() {}

		virtual ~FuzzyStateMachine(void)
		{
			clearStates();
		}

		virtual void addState(FuzzyStatePtr state)
		{
			mStates.push_back(State(false, state));
		}
        virtual void clearStates()
		{
			typename StateList::iterator itr = mStates.begin();
			for(; itr != mStates.begin(); ++itr)
			{
				delete itr->second;
			}
			mStates.clear();
		}

		virtual bool isActive(FuzzyStatePtr state)
		{
			return false; 
		}
		virtual void reset(){}

		virtual void setMaxActivity(int count){ mMaxActiveStates = count; }
		virtual int getMaxActivity() { return mMaxActiveStates; }

		float calculateActivation(void){ return 0.0f; }

		virtual void init(){}
		virtual void activate(){}
		virtual void deactivate(){}
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
		int mMaxActiveStates;
		typedef std::pair<bool, FuzzyStatePtr> State;
		typedef std::list<State> StateList;
		StateList mStates;
	};

/*	template <class Controller> void FuzzyStateMachine<Controller>::update(float elapsedTime)
	{

	}
*/
}
#endif

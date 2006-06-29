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
	template <class Controller> class FuzzyState
	{
	public:
		FuzzyState(void)
			: mParent(NULL), mController(NULL) {}

		FuzzyState<Controller>(FuzzyState* parent, Controller* controller)
			: mParent(parent), mController(controller) {}

		virtual ~FuzzyState(void){};

		virtual CeGuiString getType()=0;
		virtual void init()=0;
		virtual void activate()=0;
		virtual void deactivate()=0;
		virtual void update(const float elapsedTime)=0;

		virtual float calculateActivation()=0;

		virtual void checkBounds(float lowerBound = 0.0f, float upperBound = 1.0f)
		{
			mActivationLevel = std::max(mActivationLevel, lowerBound);
			mActivationLevel = std::min(mActivationLevel, upperBound);
		}

		float getActivationLevel() { return mActivationLevel; }
		void setActivationLevel(float activationLevel)  { mActivationLevel = activationLevel; }

		Controller* getController() { return mController; }
		void setController(Controller* controller) { mController = controller; }

		FuzzyState* getParent() { return mParent; }
		void setParent(FuzzyState* parent) { mParent = parent; }
		
	protected:
		float mActivationLevel;
		FuzzyState* mParent;
		Controller* mController;
	};
}
#endif

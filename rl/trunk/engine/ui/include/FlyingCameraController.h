/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __FlyingCameraController_H__
#define __FlyingCameraController_H__

#include "SynchronizedTask.h"

#include "UiPrerequisites.h"


namespace rl {

class CameraActor;

class _RlUiExport FlyingCameraController : public SynchronizedTask
{
	public:
        FlyingCameraController();
		FlyingCameraController(CameraActor* pActor);

		virtual ~FlyingCameraController();
		
        void setControlledActor(CameraActor* controlledActor);
        CameraActor*  getControlledActor(void);

		void run( Real elapsedTime );			

	private:
		CameraActor* mActor;

		Radian mRotX, mRotY;

		Radian mRotScale;
		float mMoveScale;

		float mTurningSpeed;
		float mMovementSpeed;
		static float TIME_DECAY;
		static float MOUSE_SENSITIVITY;
		Ogre::Real mLastEventSince;

		Vector3 mTranslateVector;
		
		void moveCamera();
		void calculateScalingFactors(Real timePassed);
		void processMouse();
		void processKeys();
};

}
#endif


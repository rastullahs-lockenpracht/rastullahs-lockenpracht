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


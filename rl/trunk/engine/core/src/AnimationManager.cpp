#include "AnimationManager.h"


template<> rl::AnimationManager* Singleton<rl::AnimationManager>::ms_Singleton = 0;

namespace rl 
{

AnimationManager::AnimationManager( ) : mAnimationList()
{

}
	
AnimationManager::~AnimationManager( )
{

}


void AnimationManager::addAnimation(AnimationState* newAnimState)
{
	bool found = false;

	for (std::list<AnimationState*>::iterator it = mAnimationList.begin(); 
			it != mAnimationList.end(); it++)
	{
		if (*it == newAnimState)
		{
			(*it)->setEnabled(true);
			found = true;
		}
	}

	if (!found)
	{
		mAnimationList.push_back(newAnimState);
		newAnimState->setEnabled(true);
	}
}

void AnimationManager::removeAnimation(AnimationState* oldAnimState)
{
	for (std::list<AnimationState*>::iterator it = mAnimationList.begin(); 
			it != mAnimationList.end(); it++)
	{
		if (*it == oldAnimState)
		{
			(*it)->setEnabled(false);
			mAnimationList.remove(*it);
			return;
		}
	}
}


void AnimationManager::run(Real timePassed)
{
	for (std::list<AnimationState*>::iterator it = mAnimationList.begin(); 
			it != mAnimationList.end(); it++)
	{
		if ((*it)->getEnabled())
			(*it)->addTime(timePassed);
	}
}

AnimationManager& AnimationManager::getSingleton(void)
{
	return Singleton<AnimationManager>::getSingleton();
}

AnimationManager* AnimationManager::getSingletonPtr(void)
{
	return Singleton<AnimationManager>::getSingletonPtr();
}


}

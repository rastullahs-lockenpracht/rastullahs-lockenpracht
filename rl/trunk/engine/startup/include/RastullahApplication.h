#ifndef __RastullahApplication_H__
#define __RastullahApplication_H__

#include <OgreSingleton.h>

namespace rl {

	class SoundSubsystem;
	class RulesSubsystem;
	class DialogSubsystem;
	class UiSubsystem;
	class CoreSubsystem;
	
	class RastullahApplication 
		: public Ogre::Singleton<RastullahApplication>
	{
	public:
		RastullahApplication();
		~RastullahApplication();
	
		static RastullahApplication& getSingleton();
		static RastullahApplication* getSingletonPtr();
	
		void run();
		void quit();
		
	private:
		CoreSubsystem* mCore;
		SoundSubsystem* mSound;
		RulesSubsystem* mRules;
		DialogSubsystem* mDialog;
		UiSubsystem* mUi;
	};	
}

#endif

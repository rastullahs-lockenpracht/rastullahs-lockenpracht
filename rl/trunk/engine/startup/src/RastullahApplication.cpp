#include "RastullahApplication.h"

#include "CoreSubsystem.h"
#include "SoundSubsystem.h"
#include "RulesSubsystem.h"
#include "DialogSubsystem.h"
#include "UiSubsystem.h"

template<> rl::RastullahApplication* 
	Ogre::Singleton<rl::RastullahApplication>::ms_Singleton = 0;

namespace rl {
		
	RastullahApplication::RastullahApplication()
	{
	   /**@todo das nach RastullahApplication
		* und RastullahApplication nach Startup. */
		mCore = new CoreSubsystem();
		Ogre::Log* log = 
			Ogre::LogManager::getSingleton().getLog( "logs/rlCore.log" );
		log->logMessage("CoreSubsystem gestartet");
		
		mSound = new SoundSubsystem();
		log->logMessage("SoundSubsystem gestartet");
		
		mRules = new RulesSubsystem();
		log->logMessage("RulesSubsystem gestartet");
		
	//	mDialog = new DialogSubsystem();
	//	log->logMessage("DialogSubsystem gestartet");
		
		mUi = new UiSubsystem();
		log->logMessage("UiSubsystem gestartet");
	}
	
	void RastullahApplication::run()
	{
		try {
			Ogre::LogManager::getSingleton().
				getLog( "logs/rlCore.log" )->logMessage("Starte...");
			mCore->startCore();
		} 
		catch(Ogre::Exception& oe) {
			rl::showError(oe.getFullDescription());
		} 
		catch(rl::Exception& re) {
			rl::showError(re.toString());
		} 
		catch(std::runtime_error& rte) {
			rl::showError(rte.what());
		} 
		catch(std::exception& exp) {
			rl::showError( exp.what() );
		} 
		catch(std::string& err) {
			rl::showError( err );
		} 
		catch(...) {
			rl::showError( "Unknown exception occured" );
		}
	
		quit();	
	}
	
	void RastullahApplication::quit()
	{
		delete mUi;
	//	delete mDialog;
		delete mRules;
		delete mSound;
		delete mCore;
		
		exit(0);
	}
	
	RastullahApplication& RastullahApplication::getSingleton()
	{
		return Singleton<RastullahApplication>::getSingleton();
	}
	
	RastullahApplication* RastullahApplication::getSingletonPtr()
	{
		return Singleton<RastullahApplication>::getSingletonPtr();
	}

}

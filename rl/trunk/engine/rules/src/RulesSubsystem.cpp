#include "RulesSubsystem.h"
#include "ActionManager.h"
#include "DsaManager.h"
#include "DsaDataLoader.h"

template <>
rl::RulesSubsystem* Singleton<rl::RulesSubsystem> ::ms_Singleton = 0;
namespace rl
{
    RulesSubsystem& RulesSubsystem::getSingleton(void)
    {
        return Singleton<RulesSubsystem>::getSingleton();
    }

    RulesSubsystem* RulesSubsystem::getSingletonPtr(void)
    {
        return Singleton<RulesSubsystem>::getSingletonPtr();
    }

    RulesSubsystem::RulesSubsystem()
    {
		//Log initialisieren
		mLog = Ogre::LogManager::getSingleton().createLog( "rlRules.log" );
	
		
		log("RlRules: Start");
        //Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));

		
        //Singletons erzeugen
        new ActionManager(); 
		log("RlRules: ActionManager erzeugt");
        new DsaManager();
		log("RlRules: DsaManager erzeugt");

		//Daten laden
		DsaDataLoader::loadData("basis.xdi");
		log("RlRules: Basisdaten geladen");
		DsaDataLoader::loadData("kalchas.xml"); // XXX: Nur zu Testzwecken
		log("RlRules: Testheld geladen");
		
		log("RlRules: Erzeugen abgeschlossen");		
    }
	
    void RulesSubsystem::log(const char *msg)
    {
        mLog->logMessage(msg);
    }
    
	void RulesSubsystem::log(const std::string& msg)
	{
		mLog->logMessage(msg);
	}

	void RulesSubsystem::log(const CeGuiString& msg)
	{
		log(msg.c_str());
	}

    RulesSubsystem::~RulesSubsystem()
    {
        delete DsaManager::getSingletonPtr();
    }
}

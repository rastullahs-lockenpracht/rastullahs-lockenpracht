#include "RulesSubsystem.h"
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
        //Zufallsgenerator initialisieren
        srand(static_cast<unsigned int>(time(NULL)));

        //Singletons erzeugen
        new DsaManager();

		//Daten laden
		DsaDataLoader::loadData("basis.xdi");
    }

    RulesSubsystem::~RulesSubsystem()
    {
        delete DsaManager::getSingletonPtr();
    }
}

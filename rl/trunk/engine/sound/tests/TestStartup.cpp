#include "MusicManager.h"
#include "SoundManager.h"
#include "Ogre.h"
#include "signal.h"
#include "SoundSubsystem.h"
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>


int main( int argc, char **argv)
{
    static std::string CONF_DIR = "./modules/common/conf/";

    #if OGRE_PLATFORM == PLATFORM_WIN32
        new Ogre::Root( 
                CONF_DIR+"plugins-win.cfg", 
                CONF_DIR+"rastullah.cfg", 
                "logs/ogre.log" );
    #elif OGRE_PLATFORM == PLATFORM_LINUX
        new Ogre::Root( 
            CONF_DIR+"plugins-linux.cfg", 
            CONF_DIR+"rastullah.cfg", 
             "logs/ogre.log" );
    #else
        new Ogre::Root( 
                CONF_DIR+"plugins-mac.cfg", 
              CONF_DIR+"rastullah.cfg", 
                "logs/ogre.log" );
    #endif
    new rl::SoundSubsystem;
  
    Ogre::ResourceManager::addCommonSearchPath("./modules/common/sound");
    Ogre::ResourceManager::addCommonSearchPath("./modules/minidemo/sound");
    
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    runner.run();
    
    return 0;
}

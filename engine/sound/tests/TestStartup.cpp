#include "MusicManager.h"
#include "SoundManager.h"
#include "Ogre.h"
#include "signal.h"
#include "SoundSubsystem.h"
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>


int main( int argc, char **argv)
{
    rl::SoundSubsystem sss;
  
#if OGRE_PLATFORM == PLATFORM_WIN32
    new Root( "plugins-win.cfg", "rastullah.cfg", "rastullah.log" );
#elif OGRE_PLATFORM == PLATFORM_LINUX
    new Root( "/etc/rl/plugins.cfg", "/etc/rl/rastullah.cfg", "rastullah.log" );
#else
    new Root( "plugins-mac.cfg", "rastullah.cfg", "rastullah.log" );
#endif
    // Erst mal um zu starten, vielleicht spaeter ueber .cfg.
    ResourceManager::addCommonSearchPath("./sound");

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    runner.run();
    
    return 0;
}

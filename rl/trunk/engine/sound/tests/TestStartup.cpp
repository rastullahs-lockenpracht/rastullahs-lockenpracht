#include "MusicManager.h"
#include "SoundManager.h"
#include "Ogre.h"
#include "signal.h"
#include "SoundSubsystem.h"
#include "CoreSubsystem.h"
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>


int main( int argc, char **argv)
{
    rl::CoreSubsystem css;
    rl::SoundSubsystem sss;
  
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    runner.run();
    
    return 0;
}

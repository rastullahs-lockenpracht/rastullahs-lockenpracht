#include <xercesc/util/XMemory.hpp> // Muss vor Ogre stehen (zumindest für VS)

#include <Ogre.h>
#include <OgreLogManager.h>
#include <stdexcept>
#include <errno.h>

#include "CoreSubsystem.h"
#include "SoundSubsystem.h"
#include "RulesSubsystem.h"
#include "DialogSubsystem.h"
#include "UiSubsystem.h"
#include "ScriptSubsystem.h"
#include "RubyInterpreter.h"
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>


#include "Exception.h"
#include <CEGUIExceptions.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #define WIN32_LEAN_AND_MEAN
    #include "windows.h"
#else
    #include "SDL.h"
#endif


int main( int argc, char **argv)
{
    rl::CoreSubsystem* core = NULL;
    rl::SoundSubsystem* sound = NULL;
    rl::RulesSubsystem* rules = NULL;
    rl::DialogSubsystem* dialog = NULL;
    rl::UiSubsystem* ui =  NULL;
    rl::ScriptSubsystem* script =  NULL;

    try {

        /**@todo das nach RastullahApplication
        * und RastullahApplication nach Startup. */
        core = new rl::CoreSubsystem();
        core->log("CoreSubsystem gestartet");

        sound = new rl::SoundSubsystem();
        core->log("SoundSubsystem gestartet");

        rules = new rl::RulesSubsystem();
        core->log("RulesSubsystem gestartet");

        dialog = new rl::DialogSubsystem();
        core->log("DialogSubsystem gestartet");

        ui = new rl::UiSubsystem();
        core->log("UiSubsystem gestartet");

        script = new rl::ScriptSubsystem();
        core->log("ScriptSubsystem gestartet");

        core->log("Starte...");
        //core->getInterpreter()->execute("load 'startup-global.rb'");

        CppUnit::TextUi::TestRunner runner;
        CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
        runner.addTest( registry.makeTest() );
        runner.run();
        core->log("Ende...");

    } 
    catch(Ogre::Exception& oe) {
        rl::showError(oe.getFullDescription());
    } 
    catch(rl::Exception& re) {
        rl::showError(re.toString());
    } 
    catch(CEGUI::Exception& ce) {
        rl::showError(ce.getMessage().c_str());
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

    try 
    {
        delete script;
        delete ui;
        delete dialog;
        delete rules;
        delete sound;
        delete core;
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

#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
    SDL_Quit();
#endif
    
    
    return 0;
}

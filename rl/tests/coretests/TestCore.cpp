#include <xercesc/util/XMemory.hpp> // Muss vor Ogre stehen (zumindest für VS)

#include <Ogre.h>
#include <OgreLogManager.h>
#include <errno.h>
#include <stdexcept>

#include "CoreSubsystem.h"
#include "DialogSubsystem.h"
#include "Logger.h"
#include "RubyInterpreter.h"
#include "RulesSubsystem.h"
#include "ScriptSubsystem.h"
#include "UiSubsystem.h"
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "Exception.h"
#include <CEGUIExceptions.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

int main(int argc, char** argv)
{
    rl::CoreSubsystem* core = NULL;
    rl::MultimediaSubsystem* sound = NULL;
    rl::RulesSubsystem* rules = NULL;
    rl::DialogSubsystem* dialog = NULL;
    rl::UiSubsystem* ui = NULL;
    rl::ScriptSubsystem* script = NULL;

    try
    {

        /**@todo das nach RastullahApplication
         * und RastullahApplication nach Startup. */
        core = new rl::CoreSubsystem();
        rl::Logger::getSingleton().log(rl::Logger::CORE, Ogre::LML_NORMAL, "CoreSubsystem gestartet");

        sound = new rl::MultimediaSubsystem();
        rl::Logger::getSingleton().log(rl::Logger::CORE, Ogre::LML_NORMAL, "MultimediaSubsystem gestartet");

        rules = new rl::RulesSubsystem();
        rl::Logger::getSingleton().log(rl::Logger::CORE, Ogre::LML_NORMAL, "RulesSubsystem gestartet");

        dialog = new rl::DialogSubsystem();
        rl::Logger::getSingleton().log(rl::Logger::CORE, Ogre::LML_NORMAL, "DialogSubsystem gestartet");

        ui = new rl::UiSubsystem();
        rl::Logger::getSingleton().log(rl::Logger::CORE, Ogre::LML_NORMAL, "UiSubsystem gestartet");

        script = new rl::ScriptSubsystem();
        rl::Logger::getSingleton().log(rl::Logger::CORE, Ogre::LML_NORMAL, "ScriptSubsystem gestartet");

        rl::Logger::getSingleton().log(rl::Logger::CORE, Ogre::LML_NORMAL, "Starte...");
        // core->getInterpreter()->execute("load 'startup-global.rb'");

        CppUnit::TextUi::TestRunner runner;
        CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
        runner.addTest(registry.makeTest());
        runner.run();
        rl::Logger::getSingleton().log(rl::Logger::CORE, Ogre::LML_NORMAL, "Ende...");
    }
    catch (Ogre::Exception& oe)
    {
        rl::showError(oe.getFullDescription());
    }
    catch (rl::Exception& re)
    {
        rl::showError(re.toString());
    }
    catch (CEGUI::Exception& ce)
    {
        rl::showError(ce.getMessage().c_str());
    }
    catch (std::runtime_error& rte)
    {
        rl::showError(rte.what());
    }
    catch (std::exception& exp)
    {
        rl::showError(exp.what());
    }
    catch (std::string& err)
    {
        rl::showError(err);
    }
    catch (...)
    {
        rl::showError("Unknown exception occured");
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
    catch (Ogre::Exception& oe)
    {
        rl::showError(oe.getFullDescription());
    }
    catch (rl::Exception& re)
    {
        rl::showError(re.toString());
    }
    catch (std::runtime_error& rte)
    {
        rl::showError(rte.what());
    }
    catch (std::exception& exp)
    {
        rl::showError(exp.what());
    }
    catch (std::string& err)
    {
        rl::showError(err);
    }
    catch (...)
    {
        rl::showError("Unknown exception occured");
    }

    return 0;
}

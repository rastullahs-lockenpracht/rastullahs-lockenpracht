/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

#ifndef __CoreSubsystem_H__
#define __CoreSubsystem_H__


#include <map>

#include "EventSource.h"
#include "EventCaster.h"
#include "CorePrerequisites.h"

namespace rl {

class RubyInterpreter;
class World;
class CoreEvent;
class CoreEventListener;
class Actor;
class ContentModule;
class ScriptWrapper;
class PhysicsManager;
class MessagePump;
class GameLoop;
class AnimationManager;
class ActorManager;
class GameEventManager;
class XmlResourceManager;
class SoundManager;
class DebugVisualsManager;
class SaveGameManager;
class JobScheduler;
class ZoneManager;
class TimeSourceManager;

typedef _RlCoreExport std::map<Ogre::String, ContentModule*> ModuleMap;

/** CoreSubsystem.
    @remarks
        It follows the Singleton Pattern,
        and initialises the game context.
*/
class _RlCoreExport CoreSubsystem : public Ogre::Singleton<CoreSubsystem>
{
public:
    /** Default Constructor */
    CoreSubsystem();
    /** Default Deconstructor */
    virtual ~CoreSubsystem();

    /** Starts the Game */
    void startCore();

    /** Renders one frame (for internal use) */
    void renderOneFrame(bool executeTasks = true);

    World* getWorld();
    void loadMap(const Ogre::String type, const Ogre::String filename,
        const Ogre::String module);

    RubyInterpreter* getRubyInterpreter();

    ContentModule* getActiveAdventureModule() const;
    ContentModule* getModule(const Ogre::String& moduleId) const;
    const ModuleMap& getAllModules() const;
    void startAdventureModule(ContentModule* module);
    void setDefaultActiveModule(const Ogre::String& moduleId);
    const Ogre::String& getDefaultActiveModule() const;
    void registerModule(ContentModule* module);

    void setDeveloperMode(bool developerMode);
    bool getDeveloperMode() const;

    /** Saves a timestamped png Screenshot
        @param sName The filename (extended with the timestamp)
    */
    void makeScreenshot(const Ogre::String& sName);

    void setScheme(const Ogre::String& schemeName);

    bool isInitialized() const;

    const Ogre::String getEngineVersionString() const;
    const Ogre::String getEngineVersionName() const;
    long         getEngineBuildNumber() const;

    /**
     * Retrieves a pointer to the current render window.
     *
     * @return pointer to render window
     */
    Ogre::RenderWindow* getRenderWindow() const;

	/// Sets the pause state of the game
	/// @param paused <code>true</code> if the game should be paused, <code>false</code> if it should be continued
	void setPaused(bool paused);

    void setTimeFactor(const Ogre::Real& factor = 1.0F);

    void _setCurrentScene(const CeGuiString& scene);
    const CeGuiString& getCurrentScene() const;

private:
    World* mWorld;
    RubyInterpreter* mRubyInterpreter;
    ModuleMap mModules;
    ContentModule* mActiveAdventureModule;
    Ogre::String mDefaultActiveModule;
    Ogre::RenderWindow* mRenderWindow;            //!< Render window for OGRE
    CeGuiString mCurrentScene;

    RL_LONGLONG mClockStartTime;

    bool mDeveloperMode;
    bool mInitialized;
    std::vector<Ogre::Technique*> mDefaultTechniques;

    // The singletons of this subsystem
    Ogre::Root* mOgreRoot;
    ScriptWrapper* mScriptWrapper;
    XmlResourceManager* mXmlResourceManager;
    PhysicsManager* mPhysicsManager;
    MessagePump* mMessagePump;
    GameLoop* mGameLoop;
    AnimationManager* mAnimationManager;
    ActorManager* mActorManager;
    GameEventManager* mGameEventManager;
    SoundManager* mSoundManager;
    DebugVisualsManager* mDebugVisualsManager;
    SaveGameManager* mSaveGameManager;
    JobScheduler* mJobScheduler;
	ZoneManager* mZoneManager;
    TimeSourceManager* mTimeSourceManager;

    /** Runs the setup methods  */
    bool initializeCoreSubsystem();

    /** Loads all needed ressources */
    void initializeResources();

    void loadPlugins();
    void unloadPlugins();

    /**
     * Sammelt alle Default-Techniques, um sie später bei jedem aktivierten Scheme
     * benutzen zu können
     */
    void updateDefaultScheme();

    void loadModule(ContentModule* module);
};

}
#endif


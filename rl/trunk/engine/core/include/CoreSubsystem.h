/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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


#include <OgreSingleton.h>
#include <OgreString.h>
#include <OgreRoot.h>
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
class GameLoopManager;
class AnimationManager;
class ActorManager;
class GameEventManager;
class XmlResourceManager;
class ConfigurationManager;
class SoundManager;

typedef _RlCoreExport std::map<Ogre::String, ContentModule*> ModuleMap;

/** CoreSubsystem. 
	@remarks		
        It follows the Singleton Pattern,
		and initialises the game context.
*/
class _RlCoreExport CoreSubsystem : protected Ogre::Singleton<CoreSubsystem>, public virtual EventSource
{
public:
	/** Default Constructor */
	CoreSubsystem();
    /** Default Deconstructor */
	virtual ~CoreSubsystem();

	/** Starts the Game */
	void startCore();
	
	/** Returns the Singleton */
	static CoreSubsystem & getSingleton();
	static CoreSubsystem * getSingletonPtr();

	World* getWorld();
	void loadMap(const Ogre::String type, const Ogre::String filename,
	    const Ogre::String module, const Ogre::String startupScript = "");

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

	
	/** Saves a timestamped jpg Screenshot
		@param sName The filename (extended with the timestamp)
	*/
	void makeScreenshot(const Ogre::String& sName);

    /** Gibt die abgelaufene Spielzeit zurück*/
	RL_LONGLONG getClock();

	/** 
	*   Setzt die abgelaufene Spielzeit wieder auf Null, die DSA-Zeit wird 
	*   aus der Basiszeit + abgelaufener Spielzeit berechnet
	*
	* @see DsaManager
	*/
	void resetClock();

	void addCoreEventListener(CoreEventListener* listener);
	void removeCoreEventListener(CoreEventListener* listener);

	void setScheme(const Ogre::String& schemeName);

	bool isInitialized() const;

private:
	World* mWorld;
	RubyInterpreter* mRubyInterpreter;
	ModuleMap mModules;
	ContentModule* mActiveAdventureModule;
	Ogre::String mDefaultActiveModule;

	RL_LONGLONG mClockStartTime;

	bool mDeveloperMode;
	bool mInitialized;
	EventCaster<CoreEvent> mCoreEventCaster;
	std::vector<Ogre::Technique*> mDefaultTechniques;

    // The singletons of this subsystem
    Ogre::Root* mOgreRoot;
    ScriptWrapper* mScriptWrapper;
    XmlResourceManager* mXmlResourceManager;
    PhysicsManager* mPhysicsManager;
    GameLoopManager* mGameLoopManager;
    AnimationManager* mAnimationManager;
    ActorManager* mActorManager;
    GameEventManager* mGameEventManager;
    ConfigurationManager* mConfigurationManager;
	SoundManager* mSoundManager;

    /** Runs the setup methods  */
	bool initializeCoreSubsystem();

	/** Loads all needed ressources */
	void initializeResources();

	void loadPlugins();
    void unloadPlugins();

    /**
     * Texturen werden nicht gefunden, wenn sie erst bei Materialnutzung erzeugt werden
     * und nicht in der Gruppe des Materials sind. Deshalb hier vorerstellen.
     * @XXX Das ist mehr oder weniger ein Hack um eine Ogre-Einschränkung herum.
     */
    void precreateTextures();

	/**
	 * Sammelt alle Default-Techniques, um sie später bei jedem aktivierten Scheme 
	 * benutzen zu können
	 */
	void updateDefaultScheme();

	/** Opens a configuration dialog */
	bool setupConfiguration();

	RL_LONGLONG getCurrentTime();
};

}
#endif


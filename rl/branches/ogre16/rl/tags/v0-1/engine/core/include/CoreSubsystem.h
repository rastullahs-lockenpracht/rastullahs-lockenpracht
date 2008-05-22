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

#include "CorePrerequisites.h"

namespace rl {

class Interpreter;
class World;

/** CoreSubsystem. 
	@remarks		
        It follows the Singleton Pattern,
		and initialises the game context.
*/
class _RlCoreExport CoreSubsystem : protected Ogre::Singleton<CoreSubsystem>
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

	void setInterpreter(Interpreter* interpreter);
	Interpreter* getInterpreter();

	const Ogre::String& getActiveAdventureModule() const;
	const Ogre::StringVector& getCommonModules() const;
	const Ogre::StringVector& getActivatableModules() const;
	void startAdventureModule(const Ogre::String& module);
	void setDefaultActiveModule(const Ogre::String& module); 
	const Ogre::String& getDefaultActiveModule() const; 
	void initializeModule(const Ogre::String& module, bool isCommon);

	void setDeveloperMode(bool developerMode);
	bool getDeveloperMode() const;

	
	/** Saves a timestamped jpg Screenshot
		@param sName The filename (extended with the timestamp)
	*/
	void  makeScreenshot(const Ogre::String& sName);

    /** Gibt die abgelaufene Spielzeit zurück*/
	RL_LONGLONG getClock();

	/** Setzt die abgelaufene Spielzeit wieder auf Null, die DSA-Zeit wird 
	*   aus der Basiszeit + abgelaufener Spielzeit berechnet
	*
	* @see DsaManager
	*/
	void resetClock();

private:  
    /** Runs the setup methods  */
	bool initializeCoreSubsystem();

	/** Loads all needed ressources */
	void initializeResources();
	void initializeModuleTextures(const std::string& module, bool isCommon);

    /**
     * Texturen werden nicht gefunden, wenn sie erst bei Materialnutzung erzeugt werden
     * und nicht in der Gruppe des Materials sind. Deshalb hier vorerstellen.
     * @XXX Das ist mehr oder weniger ein Hack um eine Ogre-Einschränkung herum.
     */
    void precreateTextures();

    void precreateMeshes(const std::string& module);

	void unloadModule(const std::string& module);
    void addSearchPath(const std::string& path, const std::string& module);

	/** Opens a configuration dialog */
	bool setupConfiguration();

	RL_LONGLONG getCurrentTime();

	World* mWorld;
	Interpreter* mInterpreter;
	Ogre::StringVector mCommonModules;
	Ogre::StringVector mActivatableModules;
	Ogre::String mActiveAdventureModule;
	Ogre::String mDefaultActiveModule;

	RL_LONGLONG mClockStartTime;

	bool mDeveloperMode;
};

}
#endif


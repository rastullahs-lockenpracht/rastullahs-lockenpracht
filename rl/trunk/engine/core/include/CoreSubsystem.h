/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
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
class _RlCoreExport CoreSubsystem : protected Singleton<CoreSubsystem>
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
	void loadMap(const String type, const String filename, const String startupScript = "");

	void setInterpreter(Interpreter* interpreter);
	Interpreter* getInterpreter();

	StringVector getActiveModules() const;
	const StringVector& getCommonModules() const;
	const StringVector& getActivatableModules() const;
	void setActiveModule(const String module);

	/** Saves a timestamped jpg Screenshot
		@param sName The filename (extended with the timestamp)
	*/
	void  makeScreenshot(const String& sName);

    static void log ( const String& msg );
	
private:  
    /** Runs the setup methods  */
	bool initializeCoreSubsystem();

	/** Loads all needed ressources */
	void initializeResources();
	void initializeModule(std::string module);
	void unloadModule(std::string module);
	void addCommonSearchPath(std::string path);	

	/** Opens a configuration dialog */
	bool setupConfiguration();

	World* mWorld;
	Interpreter* mInterpreter;
	StringVector mCommonModules;
	StringVector mActivatableModules;
	String mActiveModule;
    String mRootDir;

#if OGRE_PLATFORM == PLATFORM_LINUX
    static const String findConfRootDir();
    static const String findRastullahConf();
#endif
};

}
#endif


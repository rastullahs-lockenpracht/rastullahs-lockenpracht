#ifndef __RastullahApplication_H__
#define __RastullahApplication_H__

#include <OgreSingleton.h>

#include "CorePrerequisites.h"

namespace rl {

class Interpreter;
class World;

/** The Main Application 
	@remarks
		CoreSubsystem. 
        It follows the Singleton Pattern,
		and initialises the game context.
	@par
		None.
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

	/** Opens a configuration dialog */
	bool setupConfiguration();

private:
	World* mWorld;
	Interpreter* mInterpreter;
	StringVector mCommonModules;
	StringVector mActivatableModules;
	String mActiveModule;
};

}
#endif




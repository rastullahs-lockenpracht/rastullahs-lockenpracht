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
	void startCore(void);

	/** Returns the Singleton */
	static CoreSubsystem & getSingleton(void);
	static CoreSubsystem * getSingletonPtr(void);

	World* getWorld();

	/** Saves a timestamped jpg Screenshot
		@param sName The filename (extended with the timestamp)
	*/
	void  makeScreenshot(const String& sName);

    static void log ( const String& msg );
private:  
    /** Runs the setup methods  */
	bool initializeCoreSubsystem(void);

	/** Loads all needed ressources */
	void initializeResources(void);

	/** Opens a configuration dialog */
	bool setupConfiguration(void);

private:
	World* mWorld;
};

}
#endif




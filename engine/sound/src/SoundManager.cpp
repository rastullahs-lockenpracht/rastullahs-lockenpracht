#include "SoundManager.h"
#include <set>
#include "OgreArchiveEx.h"
#include "SoundResource.h"

using namespace std;
using namespace Ogre;

/** Das Singleton
 * @author JoSch
 * @date 04-26-2004
 */
template<> rl::SoundManager* Singleton<rl::SoundManager>::ms_Singleton = 0;
 
namespace rl {

/**
 * @return Eine Referenz auf das Singleton-Objekt.
 * @author JoSch
 * @date 04-26-2004
 */
SoundManager& SoundManager::getSingleton()
{
    return Singleton<SoundManager>::getSingleton();
}

/**
 * @return Ein Zeiger auf das Singleton-Objekt.
 * @author JoSch
 * @date 04-26-2004
 */
SoundManager* SoundManager::getSingletonPtr()
{
    return Singleton<SoundManager>::getSingletonPtr();
}

/**
 * Gibt das Suchmuster fuer die Extension zurueck.
 * @author JoSch
 * @date 06-18-2004
 */
String SoundManager::getExtension()
{
    return "*.wav";
}

/**
 * @param resName. Der Name der Soundresource, die erzeugt
 * werden soll.
 * @author JoSch
 * @date 06-18-2004
 */
Resource* SoundManager::create(const String& resName)
{
    SoundResource *newSound = new SoundResource(resName);
    return newSound;
}


}

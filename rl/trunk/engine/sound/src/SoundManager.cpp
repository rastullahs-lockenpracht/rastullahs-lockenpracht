#include "SoundManager.h"
#include <set>
#include "OgreArchiveEx.h"
#include "SoundResource.h"
#include "MusicResource.h"

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
 * Gibt die Suchmuster fuer die Extension zurueck.
 * @return Die Liste der Suchmuster
 * @author JoSch
 * @date 06-18-2004
 */
StringList SoundManager::getExtension()
{
    StringList result;
    result.push_back("*.wav");
    result.push_back("*.ogg");
    
    return result;
}

/**
 * @param resName. Der Name der Soundresource, die erzeugt
 * werden soll.
 * @author JoSch
 * @date 06-18-2004
 */
Resource* SoundManager::create(const String& resName)
{
    SndResource *newSound = 0;
    if (StringUtil::endsWith(resName, "wav", true))
    {
        newSound = new SoundResource(resName);
    } else if (StringUtil::endsWith(resName, "ogg", true)) {
        newSound = new MusicResource(resName);
    }
    return newSound;
}


}

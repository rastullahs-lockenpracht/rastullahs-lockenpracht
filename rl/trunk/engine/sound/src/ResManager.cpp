#include "ResManager.h"
#include <set>
#include "OgreArchiveEx.h"
#include "SoundResource.h"

using namespace std;
using namespace Ogre;

namespace rl {


/**
 * @author JoSch
 * @date 04-27-2004
 */
void ResManager::addSounds()
{
    set<String> list = ResourceManager::_getAllCommonNamesLike("./", getExtension());
    mutex::scoped_lock lock(mResListMutex);
    set<String>::const_iterator it;
    for(it = list.begin(); it != list.end(); it++)
    {
        try {
            add(create(*it));
        } catch(...)
        {}
    }
}

/**
 * @return Gibt den gesamten relativen Pfad der Datei zurueck.
 * @param filename Der Dateiname, den ich suche.
 * @author JoSch
 * @date 05-01-2004
 */
const String ResManager::getCommonPathByName(const String &filename)
{
#if OGRE_PLATFORM == PLATFORM_WIN32
    String delimiter = "\\";
#else
    String delimiter = "/";
#endif
    ArchiveEx::FileInfo *finfo = new ArchiveEx::FileInfo();
    vector<ArchiveEx*>::iterator j; 

    for(j = mCommonVFS.begin(); j != mCommonVFS.end(); ++j )                 
    {
        if( *j && (*j)->fileTest(filename) )
        {
            String result = (*j)->getName();
            if (!StringUtil::endsWith(result, delimiter))
            {
                result += delimiter;
            }
            result += filename;
            return result;
        }
    } 
         
    return filename;
}

/**
 * Erzeugt eine Liste von Soundnamen.
 * @return Erzeugte Namensliste.
 * @author JoSch
 * @date 06-17-2004
 */
list<String> ResManager::getSounds()
{
    list<String> result;
    ResourceMapIterator it = getResourceIterator();
    while (it.hasMoreElements())
    {
        result.push_back(it.peekNextValue()->getName());
        it.moveNext();
    }
    
    return result;
}

/**
 * @param resName. Der Name der Soundresource, die erzeugt
 * werden soll.
 * @author JoSch
 * @date 04-26-2004
 */

}

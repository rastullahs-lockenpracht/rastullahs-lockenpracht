/* ResourceManager.cpp - Basisklasse fuer die Manager.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "ResourceManager.h"
#include <set>
#include "OgreArchiveEx.h"
#include "SoundResource.h"

using namespace std;
using namespace OpenThreads;

namespace rl {


/**
 * @author JoSch
 * @date 04-27-2004
 */
void ResourceManager::addSounds()
{
    StringList extlist = getExtension();
    StringList::const_iterator cit;
    for(cit = extlist.begin(); cit != extlist.end(); cit++)
    {
        set<Ogre::String> list = ResourceManager::_getAllCommonNamesLike("./", *cit);
        set<Ogre::String>::const_iterator it;
        for(it = list.begin(); it != list.end(); it++)
        {
            try {
                mResListMutex.lock();
                add(create(*it));
                mResListMutex.unlock();
            } catch(...)
            {}
        }
    }
}

/**
 * @return Gibt den gesamten relativen Pfad der Datei zurueck.
 * @param filename Der Dateiname, den ich suche.
 * @author JoSch
 * @date 05-01-2004
 */
const Ogre::String ResourceManager::getCommonPathByName(const Ogre::String &filename)
{
#if OGRE_PLATFORM == PLATFORM_WIN32
    Ogre::String delimiter = "\\";
#else
    Ogre::String delimiter = "/";
#endif
    ArchiveEx::FileInfo *finfo = new ArchiveEx::FileInfo();
    vector<ArchiveEx*>::iterator j; 

    for(j = mCommonVFS.begin(); j != mCommonVFS.end(); ++j )                 
    {
        if( *j && (*j)->fileTest(filename) )
        {
            Ogre::String result = (*j)->getName();
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
StringList ResourceManager::getSounds()
{
    StringList result;
    ResourceMapIterator it = getResourceIterator();
    while (it.hasMoreElements())
    {
        result.push_back(it.peekNextValue()->getName());
        it.moveNext();
    }
    
    return result;
}


}

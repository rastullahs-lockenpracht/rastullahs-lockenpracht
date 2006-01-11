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
#include <set>
#include "SoundManager.h"
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
 * Standardkonstruktor
 * @author JoSch
 * @date 01-27-2005
 */
SoundManager::SoundManager() : ResourceManager()
{
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
    result.push_back("*.ogg");
    result.push_back("*.wav");
    
    return result;
}

/**
* @param resName. Der Name der Soundresource, die erzeugt
* werden soll.
* @author JoSch
* @date 06-18-2004
*/
Resource* SoundManager::createImpl(
	const String& resName, 
	ResourceHandle handle,
	const String& group,
	bool isManual, 
	ManualResourceLoader* loader, 
	const NameValuePairList* loadParams)
{
	SoundResource *newSound = 0;
	newSound = new SoundResource(this, resName, handle, group, isManual, loader);
	return newSound;
}

ResourcePtr SoundManager::create(const String& name, const String& group, 
            bool isManual, ManualResourceLoader* loader , 
            const NameValuePairList* createParams)
{
    ResourcePtr p = ResourceManager::create(name, group, isManual, loader, createParams);
    return p;
}

/**
 * @author JoSch
 * @date 04-27-2004
 */
void SoundManager::addSounds(const Ogre::String& group)
{
    StringList extlist = getExtension();
    StringList::const_iterator cit;
    for(cit = extlist.begin(); cit != extlist.end(); cit++)
    {
		StringVectorPtr sl = ResourceGroupManager::getSingleton().findResourceNames(
			group, *cit );
        StringVector::const_iterator it;
        for(it = sl->begin(); it != sl->end(); it++)
        {
            try {
                create(*it, group);
            } catch(...)
            {}
        }
    }
}

void SoundManager::add(const String& filename, const String& group)
{
	boost::mutex::scoped_lock lock(mResListMutex);
    create(filename, group);
}

/**
 * Erzeugt eine Liste von Soundnamen.
 * @return Erzeugte Namensliste.
 * @author JoSch
 * @date 06-17-2004
 */
StringList SoundManager::getSounds()
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

/* SoundManager.cpp - Spielt verschiedene Sound nach Belieben.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
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
#include <set>
#include <OgreArchiveEx.h>
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
    SoundResource *newSound = 0;
    newSound = new SoundResource(resName);
    return newSound;
}


}

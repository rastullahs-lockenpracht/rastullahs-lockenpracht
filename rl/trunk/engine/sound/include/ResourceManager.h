/* ResourceManager.h - Basisklasse fuer die Manager.
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
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "SoundPrerequisites.h"
#include <Ogre.h>
#include <list>
#include <OpenThreads/Mutex>


using namespace OpenThreads;

namespace rl {

typedef std::list<Ogre::String> StringList;

/**
 * Der ResManager verwaltet die Sounds, die das Spiel benutzt.
 * Dieses ist die abstrakte Basisklasse fuer Musik und Sound.
 * @author JoSch
 * @version 1.0
 * @date 06-18-2004
 */ 
class _RlSoundExport ResourceManager: public Ogre::ResourceManager {
    private:
        /// Ein Mutex, um das Hinzufügen der Sounds zu synchronisieren.
        Mutex mResListMutex;
    protected:
        /// Welche Dateiendung soll verwendet werden.
        virtual StringList getExtension() { return StringList(); }
    public:
        /// Alle Sounds in die Resourcenliste eintragen.
        void addSounds();
        /// Damit OpenAl++ funktioniert muss es den Pfad der Dateien wissen.
        static const Ogre::String getCommonPathByName(const Ogre::String &name);
        /// Eine Namesliste erzeugen
        StringList getSounds();
};

}
#endif

#ifndef RESMANAGER_H
#define RESMANAGER_H

#include "SoundPrerequisites.h"
#include "Ogre.h"
#include <list>
#include "boost/thread/mutex.hpp"

using namespace boost;

namespace rl {

/**
 * Der ResManager verwaltet die Sounds, die das Spiel benutzt.
 * Dieses ist die abstrakte Basisklasse fuer Musik und Sound.
 * @author JoSch
 * @version 1.0
 * @date 06-18-2004
 */ 
class _RlSoundExport ResManager: public ResourceManager {
    private:
        /// Ein Mutex, um das Hinzufügen der Sounds zu synchronisieren.
        mutex mResListMutex;
    protected:
        /// Welche Dateiendung soll verwendet werden.
        virtual Ogre::String getExtension() { return 0; }
    public:
        /// Alle Sounds in die Resourcenliste eintragen.
        void addSounds();
        /// Damit OpenAl++ funktioniert muss es den Pfad der Dateien wissen.
        static const Ogre::String getCommonPathByName(const Ogre::String &name);
        /// Eine Namesliste erzeugen
        std::list<String> getSounds();
};

}
#endif

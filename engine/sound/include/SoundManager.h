#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H


#include "SoundPrerequisites.h"
#include "ResManager.h"

namespace rl {

/**
 * Der SoundManager verwaltet die Sounds, die das Spiel benutzt.
 * Die Sounds werden geladen und entladen, je nachdem, ob die
 * Speichergrenze ueberschritten wurde.
 * @author JoSch
 * @version 1.0
 * @date 04-26-2004
 */ 
class _RlSoundExport SoundManager: public ResManager,
        public Ogre::Singleton<SoundManager> {
    protected:
        /// Welche Dateiendung soll verwendet werden.
        virtual String getExtension();
    public:
        /// Gibt das Singleton zurueck.
        static SoundManager& getSingleton();
        /// Gibt einen Zeiger auf das Singleton zurueck.
        static SoundManager* getSingletonPtr();
        /// Eine Resource erzeugen
        Resource* create(const String& resName);
};

}
#endif

/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "CorePrerequisites.h"

#include <OgreResourceManager.h>
#include <OgreSingleton.h>
#include <OgreResourceGroupManager.h>
#include <list>
#include <boost/thread/mutex.hpp>

#include "GameTask.h"

namespace rl {

class Actor;
class ListenerObject;
class SoundDriver;
class SoundManager;
class SoundUpdateTask;

typedef std::list<SoundDriver*> DriverList;

/**
 * Der SoundManager verwaltet den benutzten Treiber sowie
 * die Sounds, die das Spiel benutzt.
 * Die Sounds werden geladen und entladen, je nachdem, ob die
 * Speichergrenze ueberschritten wurde.
 * @author JoSch
 * @version 1.0
 * @date 04-26-2004
 * @version 2.0
 * @date 06-29-2005
 */ 
class _RlCoreExport SoundManager : public Ogre::ResourceManager,
        public Ogre::Singleton<SoundManager>, public GameTask 
{
    public:
        /// Gibt das Singleton zurueck.
        static SoundManager& getSingleton();
        /// Gibt einen Zeiger auf das Singleton zurueck.
        static SoundManager* getSingletonPtr();
        /// Eine Resource erzeugen
        /// Konstruktor
        SoundManager();
		virtual ~SoundManager();
        /// Alle Sounds in die Resourcenliste eintragen.
		virtual void addSounds(const Ogre::String& groupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        /// erzeugt einen Sound und fügt ihn hinzufuegen (Mit Mutex mittelbar)
        virtual void add(const Ogre::String& filename, const Ogre::String& groupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        /// Eine Namesliste erzeugen
        StringList getSounds();

        virtual Ogre::ResourcePtr create(const Ogre::String& name, 
            const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
            bool isManual = false, Ogre::ManualResourceLoader* loader = 0, 
            const Ogre::NameValuePairList* createParams = 0);

	    /// Den aktiven Listener zurückgeben.
		ListenerObject* getListener() const;
        /// Creates the listener actor.
		Actor* createListenerActor();
        /// Returns the listener actor, if it exists. Else returns NULL.
		Actor* getListenerActor();
		void _clearListenerActor();		

		/// Hole den Treiber mit dem Namen
		SoundDriver* getDriverByName(const Ogre::String &name);

		/// Hole die Soundkonfiguration
		void loadConf(const Ogre::String &filename);
		/// Schreibe die Soundkonfiguration
		void saveConf(const Ogre::String &filename) const;

		/// 3D-Sound aktualisieren.
		void update();
		/// Liste der Soundtreiber.
		const DriverList& getSoundDriverList() const;
		/// Den aktiven Treiber zurückgeben.
		SoundDriver* getActiveDriver() const;
		/// Den aktiven Treiber setzen.
		void setActiveDriver(SoundDriver *driver);

		void registerDriver(SoundDriver* driver);
		void unregisterDriver(SoundDriver* driver);
        void unloadAllDrivers();

        virtual void run( Ogre::Real elapsedTime );

        virtual const Ogre::String& getName() const;

	protected:
		virtual Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
			const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
			const Ogre::NameValuePairList* createParams);

	private:
		/// Ein MutEx, um das Hinzufügen der Sounds zu synchronisieren.
		boost::mutex mResListMutex;
		/// Welche Dateiendung soll verwendet werden.
		virtual StringList getExtension();


	    /// die Liste der bekannten und funktionierenden Treiber
		DriverList mDriverList;  
		/// Der aktuell bentutzte Soundtreiber
		SoundDriver *mActiveDriver;
		/// Der aktive Soundlistener
		ListenerObject *mActiveListener;
		Actor* mListenerActor;

		Ogre::StringVector mDrivers;
};

}
#endif

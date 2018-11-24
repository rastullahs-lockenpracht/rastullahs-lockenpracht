/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#ifndef SOUNDRESOURCE_H
#define SOUNDRESOURCE_H

#include "CorePrerequisites.h"

namespace rl
{
    /** Diese Basisklasse kapselt eine fmod-Source fuer
     * den ResourceManager von Ogre
     * @author Josch
     * @date 06-19-2004
     * @date 07-23-2004
     * @date 10-10-2004
     * @date 06-26-2005
     * @version 4.0
     */
    class _RlCoreExport SoundResource : public Ogre::Resource
    {
    public:
        /// Der Standardkonstruktor
        SoundResource(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
            const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader);
        /// Der Destruktor
        virtual ~SoundResource();
        /// Den Datenstrom zurueckgeben
        const Ogre::DataStreamPtr& getDataStream() const;

    protected:
        /// Laedt die Soundquelle.
        virtual void loadImpl();
        /// Entlaedt die Soundquelle.
        virtual void unloadImpl();
        /// Bestimmt die Groesse im Speicher (wird erst nach dem Laden aufgerufen)
        virtual size_t calculateSize() const;

    private:
        /// Unsere Daten von Ogres ResourceManager.
        Ogre::DataStreamPtr mDataStream;
    };

    using SoundResourcePtr = Ogre::SharedPtr<SoundResource>;
}
#endif

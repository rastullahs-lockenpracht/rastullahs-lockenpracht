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

#ifndef __WriteableFileSystemArchive_H__
#define __WriteableFileSystemArchive_H__

#include "CommonPrerequisites.h"

#include "OgreFileSystem.h"

namespace rl {

    /** Specialisation of the Archive class to allow reading of files from 
        filesystem folders / directories.
    */
    class _RlCommonExport WriteableFileSystemArchive : public Ogre::FileSystemArchive 
    {
    protected:
    public:
        WriteableFileSystemArchive(const Ogre::String& name, const Ogre::String& archType );
        virtual ~WriteableFileSystemArchive();

        /// @copydoc Archive::open
        Ogre::DataStreamPtr open(const Ogre::String& filename) const;
    };

    /** Specialisation of ArchiveFactory for FileSystem files. */
    //class _OgrePrivate WriteableFileSystemArchiveFactory : public ArchiveFactory
    class _RlCommonExport WriteableFileSystemArchiveFactory : public Ogre::ArchiveFactory
    {
    public:
        virtual ~WriteableFileSystemArchiveFactory() {}
        /// @copydoc FactoryObj::getType
        const Ogre::String& getType(void) const;
        /// @copydoc FactoryObj::createInstance
        Ogre::Archive *createInstance( const Ogre::String& name ) 
        {
            return new WriteableFileSystemArchive(name, "WriteableFileSystem");
        }
        /// @copydoc FactoryObj::destroyInstance
        void destroyInstance( Ogre::Archive* arch) { delete arch; }
    };


}

#endif

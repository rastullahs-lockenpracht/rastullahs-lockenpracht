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

#include "stdinc.h"

#include "OgreStableHeaders.h"
#include "OgreLogManager.h"
#include "OgreException.h"
#include "OgreStringVector.h"
#include "OgreRoot.h"

#include <sys/types.h>
#include <sys/stat.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#   include "OgreSearchOps.h"
#   include <sys/param.h>
#   define MAX_PATH MAXPATHLEN
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   include <windows.h>
#   include <direct.h>
#   include <io.h>
#endif

#include "WriteableFileSystemArchiv.h"
#include "WriteableFileStreamDataStream.h"

using namespace Ogre;

namespace rl 
{
    static bool is_reserved_dir (const char *fn)
    {
        return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
    }

    static bool is_absolute_path(const char* path)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        if (isalpha(uchar(path[0])) && path[1] == ':')
            return true;
#endif
        return path[0] == '/' || path[0] == '\\';
    }

    static String concatenate_path(const String& base, const String& name)
    {
        if (base.empty() || is_absolute_path(name.c_str()))
            return name;
        else
            return base + '/' + name;
    }



    WriteableFileSystemArchive::WriteableFileSystemArchive(const Ogre::String& name, const Ogre::String& archType ) : Ogre::FileSystemArchive(name, archType)
    {
    }

    WriteableFileSystemArchive::~WriteableFileSystemArchive()
    {
    }

    DataStreamPtr WriteableFileSystemArchive::open(const String& filename) const
    {
        String full_path = concatenate_path(mName, filename);

        // Use filesystem to determine size 
        // (quicker than streaming to the end and back)
        struct stat tagStat;
	int ret = stat(full_path.c_str(), &tagStat);
        assert(ret == 0 && "Problem getting file size" );

        // Always open in binary mode
        std::fstream *origStream = new std::fstream();
        origStream->open(full_path.c_str(), std::ios::in | std::ios::out | std::ios::binary);

        // Should check ensure open succeeded, in case fail for some reason.
        if (origStream->fail())
        {
            delete origStream;
            OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
                "Cannot open file: " + filename,
                "WriteableFileSystemArchive::open");
        }

        /// Construct return stream, tell it to delete on destroy
        WriteableFileStreamDataStream* stream = new WriteableFileStreamDataStream(filename,
            origStream, tagStat.st_size, true);
        return DataStreamPtr(stream);
    }
    
    const String& WriteableFileSystemArchiveFactory::getType(void) const
    {
        static String name = "FileSystem";
        return name;
    }

}

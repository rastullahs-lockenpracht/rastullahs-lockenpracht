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

#include <sys/types.h>
#include <sys/stat.h>

#include "WriteableFileSystemArchiv.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#   include <sys/param.h>
#   define MAX_PATH MAXPATHLEN
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   include <windows.h>
#   include <direct.h>
#   include <io.h>
#endif

#include "WriteableDataStream.h"

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
        LOG_MESSAGE(Logger::COMMON, "Opening WriteableFileStreamDataStream: " + filename);
        String full_path = concatenate_path(mName, filename);

        // Always open in binary mode
        std::fstream *origStream = new std::fstream();
        origStream->open(full_path.c_str(), std::ios::in | std::ios::out | std::ios::binary);

        // Should check ensure open succeeded, in case fail for some reason.
        if (origStream->fail())
        {
            delete origStream;
            OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
                "Cannot open or create file: " + filename,
                "WriteableFileSystemArchive::open");
        }

        /// Construct return stream, tell it to delete on destroy
        WriteableFileStreamDataStream* stream = new WriteableFileStreamDataStream(filename,
            origStream, true);
        return DataStreamPtr(stream);
    }
    
    const String& WriteableFileSystemArchiveFactory::getType(void) const
    {
        static String name = "WriteableFileSystem";
        return name;
    }

}

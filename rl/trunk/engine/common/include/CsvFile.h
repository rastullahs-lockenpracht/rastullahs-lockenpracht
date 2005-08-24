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

#ifndef __CSVFILE_H__
#define __CSVFILE_H__

#include <fstream>
#include <vector>

#include "CommonPrerequisites.h"

namespace rl {

    ///@todo in separate CPP/H-Files aufteilen.
    class _RlCommonExport CsvFile {
    public:
		CsvFile(const std::string& filename);
        ~CsvFile();

        int getValueAsInt(int row, int col);
        std::string getValueAsString(int row, int col);

        int getRowCount() const;
       

    private:
        typedef std::vector<std::string> FileRow;
        std::vector<FileRow> mFileContent;

        ///@todo funktioniert nur mit newline am dateiende. reparieren.
        void read(std::istream& in);
    };
}

#endif

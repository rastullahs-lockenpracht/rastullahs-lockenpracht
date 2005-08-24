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

#include "CsvFile.h"
#include "Exception.h"

using namespace std;

namespace rl {

CsvFile::CsvFile(const string& filename) : mFileContent()
{
    ifstream f(filename.c_str());
    if ((f.rdstate() & (ios::badbit | ios::failbit)) != 0)
    {
		Throw(rl::Exception, string("Konnte Datei nicht oeffnen. ") + filename);
    }
    read(f);
    f.close();
}

CsvFile::~CsvFile()
{
}

int CsvFile::getValueAsInt(int row, int col)
{
    return atoi(getValueAsString(row, col).c_str());
}

string CsvFile::getValueAsString(int row, int col)
{
    return (mFileContent[row])[col];
}

int CsvFile::getRowCount() const
{
    ///@todo XXX noch h?ngt hinten eine Leerzeile dran.
    return static_cast<int>(mFileContent.size()) - 1;
}

///@todo funktioniert nur mit newline am dateiende. reparieren.
void CsvFile::read(istream& in)
{
    char c;
    char buf[1024];
    int bidx = 0;
    FileRow row;

    while (in.read(&c, 1)) {
        switch (c) {
        case '\r':
            break;
        case '\n':
            buf[bidx] = 0;
            row.push_back(buf);
            mFileContent.push_back(row);
            bidx = 0;
            row.clear();
            break;
        case ',':
            buf[bidx] = 0;
            bidx = 0;
            row.push_back(buf);
            break;
        default:
            buf[bidx++] = c;
        }
    }
}

}


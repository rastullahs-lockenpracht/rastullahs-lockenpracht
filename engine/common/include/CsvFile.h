#ifndef __CSVFILE_H__
#define __CSVFILE_H__

#include <stdio.h>
#include <fstream>
#include <vector>

#include "RastullahPrerequisites.h"
#include "Exception.h"

using namespace std;

namespace rl {

    ///@todo in separate CPP/H-Files aufteilen.
    class CsvFile {
    public:
        CsvFile(const string& filename) : mFileContent()
        {
            ifstream f(filename.c_str());
            if ((f.rdstate() & (ios::badbit | ios::failbit)) != 0)
            {
                Throw(Exception, string("Konnte Datei nicht ?ffnen. ") + filename);
            }
            read(f);
            f.close();
        }

        ~CsvFile()
        {
        };

        int getValueAsInt(int row, int col)
        {
            return atoi(getValueAsString(row, col).c_str());
        }

        string getValueAsString(int row, int col)
        {
            return (mFileContent[row])[col];
        }

        int getRowCount() const
        {
            ///@todo XXX noch h?ngt hinten eine Leerzeile dran.
            return static_cast<int>(mFileContent.size()) - 1;
        }

    private:
        typedef vector<string> FileRow;
        vector<FileRow> mFileContent;

        ///@todo funktioniert nur mit newline am dateiende. reparieren.
        void read(istream& in)
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
    };
}

#endif

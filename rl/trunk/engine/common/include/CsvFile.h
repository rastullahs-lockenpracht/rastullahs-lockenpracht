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

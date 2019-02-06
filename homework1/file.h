// Anna Pankiewicz

#ifndef FILE_H
#define FILE_H

#include <vector>

#include "permissions.h"
#include "properties.h"

class File
{
    private:
        Properties prop;

        File* parent;

        std::vector<File> sub_directory;
        std::vector<File> files;

    public:
        File() {}

        File(std::string name, bool type) {
            prop.name = name;
            prop.is_file = type;
        }

        ~File() {}

        void setName(const std::string& name);
        std::string getName() const;

        void setFileType(const bool type);
};

#endif
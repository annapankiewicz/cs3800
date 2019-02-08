// Anna Pankiewicz

#ifndef FILE_H
#define FILE_H

#include <vector>
#include <ctime>
#include <string>

#include "permissions.h"
#include "properties.h"

class File
{
    private:
        Properties prop;

        File* parent;

    public:
        std::vector<File> files;

        File() {}

        File(std::string name, bool type) {
            prop.name = name;
            prop.is_file = type;
        }

        ~File() {}

        void setName(const std::string& name);
        std::string getName() const;

        void setFileType(const bool type);

        std::string longListing();
};

#endif
// Anna Pankiewicz

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <string>
#include <ctime>

#include "permissions.h"

class Properties
{
    private:
        // Permissions permissions;
        std::string owner;
        std::string group;
        time_t timestamp;
        std::string name;
        bool is_file;

    public:
        // initialize the file/directory's stuff with defaults
        Properties() {
            // TODO(anna): add permissions when it's actually possible
            owner = "root";
            group = "root";
            time(&timestamp);
            is_file = false;
        }

        void setName(std::string name);
        std::string getName() const;

        void setFileType(const bool type);

        std::string longListing(const Properties& p);
};

#endif
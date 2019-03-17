// Anna Pankiewicz

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <string>
#include <ctime>

#include "permissions.h"

struct Properties
{
    Permissions permissions;
    int num_links;
    std::string owner;
    std::string group;
    int file_size;
    time_t timestamp;
    std::string name;
    bool is_file;

    // initialize the file/directory's properties with defaults
    Properties() {
        permissions.updatePermissions(644);
        num_links = 1;
        owner = "anna";
        group = "users";
        file_size = 1024;
        time(&timestamp);
        is_file = false;
    }
};

#endif
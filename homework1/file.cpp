// Anna Pankiewicz

#include "file.h"

void File::setName(const std::string& name) {
    prop.name = name;
}

std::string File::getName() const {
    return prop.name;
}

void File::setFileType(const bool type) {
    prop.is_file = type;
}

std::string File::longListing() {

    std::string listing;

    // postponing adding permissions until permissions actually work

    // get the owner and group names
    listing += prop.owner + "\t";
    listing += prop.group + "\t";

    // make timestamp human readable and add it
    struct tm * time;
    char buffer[80];
    time = localtime(&prop.timestamp);
    strftime(buffer, 80, "%b %d %G %R", time);
    listing += buffer;

    // add the name
    listing += "\t" + prop.name;

    // make a directory look like a directory if applicable
    if(!prop.is_file) {
        listing += '\\';
    }

    return listing;
}
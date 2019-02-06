// Anna Pankiewicz

#include "properties.h"

void Properties::setName(std::string name) {
    name = name;
    return;
}

std::string Properties::getName() const {
    return name;
}

void Properties::setFileType(const bool type) {
    is_file = type;
    return;
}

std::string Properties::longListing(const Properties& p) {
    return "hi";
}
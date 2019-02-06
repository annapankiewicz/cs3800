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
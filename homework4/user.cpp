// Anna Pankiewicz

#include "user.h"

void User::setName(std::string name) {
    u_name = name;
}

std::string User::getName() const {
    return u_name;
}
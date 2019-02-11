// Anna Pankiewicz

#include "permissions.h"

Permissions::Permissions() {
    for(int i = 0; i < PERMISSION_SIZE; i++) {
        permissions[i] = false;
    }
}

void Permissions::updatePermissions(int permission_code) {

    // get each of the individual digits
    int access[MODE_SIZE];
    if((std::to_string(permission_code)).length() <= 3) {
        for(int i = MODE_SIZE-1; i >= 0; i--) {
            access[i] = permission_code % 10;
            permission_code /= 10;
        }
    }

    for(int i = 0; i < MODE_SIZE; i++) {
        if(!validDigit(access[i])) {
            std::cout << "error: invalid permissions" << std::endl;
            return;
        }
    }

    // get each set of permission digits according to group
    std::string perm_string;
    for(int i = 0; i < MODE_SIZE; i++) {
        perm_string = std::bitset<3>(access[i]).to_string() + perm_string;
    }

    // assign the appropriate permissions to the actual permissions
    for(int i = 0; i < PERMISSION_SIZE; i++) {
        permissions[i] = std::stoi(std::string(1, perm_string[i]));
    }

    return;
}

void Permissions::getPermissions() const {
    return;
}

bool Permissions::validDigit(const int digit) {
    return ((digit>=0) && (digit<=7));
}
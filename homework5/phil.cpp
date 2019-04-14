#include "phil.h"

int Phil::getID() {
    return id;
}

bool Phil::isLeftAvailable() {
    return leftAvailable;
}

bool Phil::isRightAvailable() {
    return rightAvailable;
}

void Phil::setLeftAvailable(const bool is_available){
    leftAvailable = is_available;
}

void Phil::setRightAvailable(const bool is_available) {
    rightAvailable = is_available;
}
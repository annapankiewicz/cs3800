// Anna Pankiewicz

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
using namespace std;

class Filesystem
{
    private:
        string permissions;
        string owner;
        string group;
        int size;
        string time_stamp;
        string name;

    public:
        Filesystem() {}
};

#endif
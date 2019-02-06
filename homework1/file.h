// Anna Pankiewicz

#ifndef FILE_H
#define FILE_H

#include <string>
using namespace std;

class File
{
    private:
        string permissions;
        string owner;
        string group;
        int size;
        string time_stamp;
        string name;
        bool is_file;

    public:
        File() {}

        File(string n, bool type) {
            name = n;
            is_file = type;
        }

        ~File() {}

        string getName();
};

#endif
// Anna Pankiewicz

#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <string>
#include <vector>
#include <bitset>
#include <iostream>

class Permissions
{
    private:
        static const int MODE_SIZE = 3;
        static const int PERMISSION_SIZE = 9;

        bool permissions[PERMISSION_SIZE];

        enum modes {
            READ = 0,
            WRITE = 1,
            EXECUTE = 2
        };

    public:
        Permissions();

        void updatePermissions(int permissions);

        std::string getPermissions() const;

        bool validDigit(const int digit);

};

#endif
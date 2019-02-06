// Anna Pankiewicz

#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <string>

class Permissions
{
    private:

    public:
        Permissions();

        void updatePermissions(const std::string permissions);

        void getPermissions() const;

        bool validDigit(const int digit);

};

#endif
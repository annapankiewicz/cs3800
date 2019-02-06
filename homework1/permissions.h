// Anna Pankiewicz

#ifndef PERMISSIONS_H
#define PERMISSIONS_H

class Permissions
{
    private:

    public:
        Permissions();

        void updatePermissions(const string permissions);

        void getPermissions() const;

        bool validDigit(const int digit);

};

#endif
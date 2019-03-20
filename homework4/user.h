// Anna Pankiewicz

#ifndef USER_H
#define USER_H

#include <algorithm>
#include <string>
#include <vector>

class User
{
    private:
        std::string u_name;

    public:
        std::vector<std::string> u_groups;

        User() {
            u_name = "default_user";
            u_groups.push_back("users");
        }

        User(std::string name) {
            u_name = name;
            u_groups.push_back("users");
        }

        User(std::string name, std::vector<std::string> groups) {
            u_name = name;
            u_groups.push_back("users");
            u_groups.insert(u_groups.end(), groups.begin(), groups.end());
        }

        void setName(std::string name);
        std::string getName() const;

        void setGroup(std::string group_name);
        std::string getGroup() const;

        bool matchGroup(std::string group_name);

};

#endif
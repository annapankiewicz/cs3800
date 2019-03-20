// Anna Pankiewicz

#include "user.h"

void User::setName(std::string name) {
    u_name = name;
}

std::string User::getName() const {
    return u_name;
}

void User::setGroup(std::string group_name) {
    // choosing to not care about adding a user to a group
    // they're already a member of
    u_groups.push_back(group_name);
    std::sort(u_groups.begin(), u_groups.end());
    u_groups.erase(std::unique(u_groups.begin(),
                               u_groups.end()),
                               u_groups.end());
}

std::string User::getGroup() const {
    return u_groups[0];
}

// could easily refactor this for more widespread use in shell,
// there's a lot of repeated code
bool User::matchGroup(std::string group_name) {
    std::vector<std::string>::iterator it;

    it = std::find_if(u_groups.begin(),
                      u_groups.end(),
                      [&group_name](std::string const& target)
                      {
                          // does it exist
                          return (target == group_name);
                      }
    );

    if(it != u_groups.end()) {
        return true;
    }
    else { return false; }
}
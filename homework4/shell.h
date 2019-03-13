// Anna Pankiewicz

#ifndef SHELL_H
#define SHELL_H

#include <sstream>
#include <algorithm>

#include "file.h"
#include "properties.h"
#include "user.h"

class Shell
{
    private:
        File root;
        File* current_dir;
        User* current_user;
        std::vector<User> users;
        std::vector<std::string> s_groups;

    public:
        Shell();

        void process(std::string& command);

        void parseCommand(std::string command, std::vector<std::string>& result);
        void parseGroups(std::string command, std::vector<std::string>& result);

        void ls(std::vector<std::string> command);
        void cd(std::vector<std::string> command);
        void pwd(std::vector<std::string> command);
        void mkdir(std::vector<std::string> command);
        void rmdir(std::vector<std::string> command);
        void rm(std::vector<std::string> command);
        void chmod(std::vector<std::string> command);
        void touch(std::vector<std::string> command);
        void useradd(std::vector<std::string> command);
        void usermod(std::vector<std::string> command);
        void chown(std::vector<std::string> command);
        void chgrp(std::vector<std::string> command);
        void execute(std::vector<std::string> command);
        void groups(std::vector<std::string> command);
        void groupadd(std::vector<std::string> command);
        void userdel(std::vector<std::string> command);

};

#endif
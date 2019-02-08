// Anna Pankiewicz

#ifndef SHELL_H
#define SHELL_H

#include "file.h"
#include "properties.h"

class Shell
{
    private:
        File root;
        File* current_dir;

    public:
        Shell();

        void process(std::string& command);

        void parseCommand(std::string command, std::vector<std::string>& result);

        void ls(std::vector<std::string> command);
        void cd(std::vector<std::string> command);
        void pwd(std::vector<std::string> command);
        void mkdir(std::vector<std::string> command);
        void rmdir(std::vector<std::string> command);
        void rm(std::vector<std::string> command);
        void chmod(std::vector<std::string> command);
        void touch(std::vector<std::string> command);
};

#endif
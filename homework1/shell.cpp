// Anna Pankiewicz

#include "shell.h"
#include <iostream>
#include <iomanip>
#include <vector>

Shell::Shell() {
    // initialize directory structure and "create" root directory
    root.setName("root");
    root.setFileType(false);
    current_dir = &root;
}

void Shell::process(std::string& command) {

    std::vector<std::string> cmd;

    parseCommand(command, cmd);

    if(cmd.size() != 0) {
        if(cmd[0] == "ls") {
            // ls command, including the -l option
        }
        else if (cmd[0] == "cd") {
            // cd command
        }
        else if (cmd[0] == "pwd") {
            // pwd command
        }
        else if (cmd[0] == "mkdir") {
            // mkdir command woohoo
        }
        else if (cmd[0] == "rmdir") {
            // rmdir command
        }
        else if (cmd[0] == "rm") {
            // rm command
        }
        else if (cmd[0] == "chmod") {
            // chmod command
        }
        else if (cmd[0] == "touch") {
            // touch command
        }
        else if ((cmd[0] == "exit") || (cmd[0] == "quit")) {
            std::cout << "exiting shell emulation..." << std::endl;
        }
        else {
            std::cout << "error: command not found" << std::endl;
        }
    }
    else { // command has to be at least something
        return;
    }

    return;
}

void Shell::parseCommand(std::string command, std::vector<std::string>& result) {

    // split string on whitespace & get each piece of the command
    std::istringstream stream(command);
    for(std::string command; stream >> command;)
        result.push_back(command);

    // temporary sanity check to make sure this is working the way I want
    std::vector<std::string>::iterator it;

    for(it=result.begin(); it!=result.end(); it++) {
        std::cout << " " << *it << std::endl;
    }

    return;
}
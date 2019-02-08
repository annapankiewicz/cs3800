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
            ls(cmd);
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

    return;
}

void Shell::ls(std::vector<std::string> command) {

    // process just a normal ls
    if(command.size() == 1) {
        for(int i = 0; i < current_dir->files.size(); i++) {
            std::cout << current_dir->files[i].getName() << "\t";
        }
    }
    // do a long listing
    else if(command.size() == 2) {
        if(command[1] == "-l") {
            for(int i = 0; i < current_dir->files.size(); i++) {
                std::cout << current_dir->files[i].longListing() << std::endl;
            }
        }
    }
    // something is quite wrong
    else {
        std::cout << "error: invalid ls command" << std::endl;
    }

    return;
}

void Shell::cd(std::vector<std::string> command) {

    return;
}

void Shell::pwd(std::vector<std::string> command) {

    return;
}

void Shell::mkdir(std::vector<std::string> command) {

    return;
}

void Shell::rmdir(std::vector<std::string> command) {

    return;
}

void Shell::rm(std::vector<std::string> command) {

    return;
}

void Shell::chmod(std::vector<std::string> command) {

    return;
}

void Shell::touch(std::vector<std::string> command) {

    return;
}
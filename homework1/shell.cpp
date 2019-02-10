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
            touch(cmd);
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
        std::cout << std::endl;
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

    // gotta have a directory to go to
    if(command.size() != 2) {
        std::cout << "error: invalid cd command" << std::endl;
        return;
    }

    if(command[1] == "..") {
        // assuming we can't cd .. past root
        if((current_dir->getName() == "root")) {
            std::cout << "error: already in root directory" << std::endl;
        }
        else {
            // set working directory to parent
            current_dir = current_dir->getParent();
        }
    }
    else {
        File* target = NULL;
        std::string target_dir = command[1];
        bool found = false;

        // directory has to exist
        for(int i = 0; i < current_dir->files.size(); i++) {
            if(current_dir->files[i].getName() == target_dir) {
                found = true;
                target = &(current_dir->files[i]);
            }
        }

        if(found) {
            // set parent pointer
            target->setParent(current_dir);
            // update current working directory pointer
            current_dir = target;
        }
        else {
            std::cout << "error: invalid directory" << std::endl;
            return;
        }
    }

    return;
}

void Shell::pwd(std::vector<std::string> command) {

    return;
}

void Shell::mkdir(std::vector<std::string> command) {

    // error checking, gotta have a directory name to make
    if(command.size() != 2) {
        std::cout << "error: invalid mkdir command" << std::endl;
        return;
    }

    std::string filename = command[1];
    bool found = false;

    // TODO(anna): if there's time, refactor touch & mkdir;
    //             there's way too much repetition
    // see if there's already a directory with the same name
    for(int i = 0; i < current_dir->files.size(); i++) {
        if(current_dir->files[i].getName() == filename) {
            found = true;
        }
    }

    if(found) {
        std::cout << "error: cannot create directory with duplicate name" << std::endl;
        return;
    }
    else {
        File new_file(filename, false);
        new_file.setParent(current_dir);
        current_dir->files.push_back(new_file);
    }

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

    if(command.size() != 2) {
        std::cout << "error: must provide target file" << std::endl;
        return;
    }

    Properties target;
    std::string filename = command[1];
    bool found = false;

    // check to see if the file already exists in the directory we're in
    for(int i = 0; i < current_dir->files.size(); i++) {
        if(current_dir->files[i].getName() == filename) {
            found = true;
            target = current_dir->files[i].getProp();
        }
    }

    // if the file is found, update the timestamp
    if(found) {
        // TODO(anna): this still seems to not actually update the timestamp
        time(&target.timestamp);
    }
    // if the file's not found, create it
    // also assuming we can't make a file with the same name as a directory
    else {
        File new_file(filename, true);
        new_file.setParent(current_dir);
        current_dir->files.push_back(new_file);
    }

    return;
}
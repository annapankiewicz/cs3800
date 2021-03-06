// Anna Pankiewicz

#include "shell.h"
#include <iostream>
#include <iomanip>
#include <vector>

Shell::Shell() {
    // initialize directory structure and "create" root directory
    root.setName("root");
    root.setFileType(false);
    root.setParent(NULL);
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
            cd(cmd);
        }
        else if (cmd[0] == "pwd") {
            pwd(cmd);
        }
        else if (cmd[0] == "mkdir") {
            mkdir(cmd);
        }
        else if (cmd[0] == "rmdir") {
            rmdir(cmd);
        }
        else if (cmd[0] == "rm") {
            rm(cmd);
        }
        else if (cmd[0] == "chmod") {
            chmod(cmd);
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
        else {
            std::cout << "error: unrecognized option" << std::endl;
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
            if((current_dir->files[i].getName() == target_dir) &&
               (current_dir->files[i].getFileType() == false)) {
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

    if(command.size() != 1) {
        std::cout << "error: invalid pwd command" << std::endl;
    }
    else {
        std::string path = "/";
        File* target = current_dir;
        while((target->getParent()) != NULL) {
            path = "/" + target->getName() + path;
            target = target->getParent();
        }
        path = "/root" + path;
        std::cout << path << std::endl;
    }
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

    if(command.size() != 2) {
        std::cout << "error: invalid rmdir command" << std::endl;
        return;
    }

    // TODO(anna): additionally try to refactor rmdir and rm if there's time,
    //             lots of repitition like touch/mkdir

    // try to find said directory to remove
    std::string target_name = command[1];
    std::vector<File>::iterator it;

    it = std::find_if(current_dir->files.begin(),
                      current_dir->files.end(),
                      [&target_name](File const& target)
                      {
                          // make sure it's a directory and has the right name
                          return ((target.getName() == target_name) &&
                                  (target.getFileType() == false));
                      }
    );

    // if the file is found, remove it
    if(it != current_dir->files.end()) {
        int target_index = std::distance(current_dir->files.begin(), it);
        current_dir->files.erase(current_dir->files.begin() + target_index);
    }
    else {
        std::cout << "error: no such directory to remove" << std::endl;
    }

    return;
}

void Shell::rm(std::vector<std::string> command) {

    if(command.size() != 2) {
        std::cout << "error: invalid rm command" << std::endl;
        return;
    }

    std::string target_name = command[1];
    std::vector<File>::iterator it;

    it = std::find_if(current_dir->files.begin(),
                      current_dir->files.end(),
                      [&target_name](File const& target)
                      {
                          // does it exist & is it a file
                          return ((target.getName() == target_name) &&
                                  (target.getFileType() == true));
                      }
    );

    if(it != current_dir->files.end()) {
        int target_index = std::distance(current_dir->files.begin(), it);
        current_dir->files.erase(current_dir->files.begin() + target_index);
    }
    else {
        std::cout << "error: no such file to remove" << std::endl;
    }

    return;
}

void Shell::chmod(std::vector<std::string> command) {

    if(command.size() != 3) {
        std::cout << "error: must provide permissions and target file" << std::endl;
        return;
    }

    // make sure the thing exists
    std::string target_name = command[2];
    std::vector<File>::iterator it;

    it = std::find_if(current_dir->files.begin(),
                      current_dir->files.end(),
                      [&target_name](File const& target)
                      {
                          // does it exist
                          return (target.getName() == target_name);
                      }
    );

    if(it != current_dir->files.end()) {
        // where is the thing
        int target_index = std::distance(current_dir->files.begin(), it);
        // do the thing
        current_dir->files[target_index].getProp()->permissions.
            updatePermissions(stoi(command[1]));

    }
    else {
        std::cout << "error: file not found" << std::endl;
    }

    return;
}

void Shell::touch(std::vector<std::string> command) {

    if(command.size() != 2) {
        std::cout << "error: must provide target file" << std::endl;
        return;
    }

    Properties* target;
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
        time(&target->timestamp);
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
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

    default_user.setName("anna");
    current_user = &default_user;
    users.push_back(default_user);
    s_groups.push_back("users");
}

/* -------------- Input Processing Functions -------------- */

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
        else if (cmd[0] == "useradd") {
            useradd(cmd);
        }
        else if (cmd[0] == "usermod") {
            usermod(cmd);
        }
        else if (cmd[0] == "chown") {
            chown(cmd);
        }
        else if (cmd[0] == "chgrp") {
            chgrp(cmd);
        }
        else if (cmd[0].substr(0,2) == "./") {
            execute(cmd);
        }
        else if (cmd[0] == "groups") {
            groups(cmd);
        }
        else if (cmd[0] == "groupadd") {
            groupadd(cmd);
        }
        else if (cmd[0] == "userdel") {
            userdel(cmd);
        }
        else if (cmd[0] == "switchto") {
            switchto(cmd);
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

void Shell::parseGroups(std::string command, std::vector<std::string>& result) {
    std::istringstream stream(command);
    std::string group;

    while(stream.good()) {
        std::string substring;
        std::getline(stream, substring, ',');
        result.push_back(substring);
    }
    return;
}

/* -------------- Helper Functions -------------- */

bool Shell::checkIfUserHasPermissions(File file_to_check, int access_mode) {

    bool access = false;

    // check public access first
    access = file_to_check.getProp()->permissions.getPermissions(access_mode+6);

    // check if the user is owner and has access to do the mode specified
    if((file_to_check.getProp()->owner) == (current_user->getName())) {
        access = file_to_check.getProp()->permissions.getPermissions(access_mode);
    }

    // if that fails and user doesn't have permission yet,
    // need to check if user belongs to the owning group
    else if((current_user->matchGroup(file_to_check.getProp()->group)) && !access) {
        access = file_to_check.getProp()->permissions.getPermissions(access_mode+3);
    }

    return access;
}

std::string Shell::getCurrentUser() {
    return current_user->getName();
}

/* -------------- Command Functions -------------- */

void Shell::ls(std::vector<std::string> command) {

    // process just a normal ls
    if(command.size() == 1) {
        for(int i = 0; i < current_dir->files.size(); i++) {
            if(checkIfUserHasPermissions(current_dir->files[i], 0))
                std::cout << current_dir->files[i].getName() << "\t";
        }
        std::cout << std::endl;
    }
    // do a long listing
    else if(command.size() == 2) {
        if(command[1] == "-l") {
            for(int i = 0; i < current_dir->files.size(); i++) {
                if(checkIfUserHasPermissions(current_dir->files[i], 0))
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
        File new_file(filename,
                      false,
                      current_user->getName(),
                      current_user->getGroup());
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
        if(checkIfUserHasPermissions(*it, 1)) {
            int target_index = std::distance(current_dir->files.begin(), it);
            current_dir->files.erase(current_dir->files.begin() + target_index);
        }
        else {
            std::cout << "error: invalid permissions" << std::endl;
        }
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
        File new_file(filename,
                      true,
                      current_user->getName(),
                      current_user->getGroup());
        new_file.setParent(current_dir);
        current_dir->files.push_back(new_file);
    }

    return;
}

void Shell::useradd(std::vector<std::string> command) {
    if(command.size() == 1) {
        std::cout << "error: must specify user and optionally groups" << std::endl;
        return;
    }

    if(command[1] == "-G") {
        if(command.size() != 4) {
            std::cout << "error: must specify groups and username" << std::endl;
            return;
        }

        std::string username = command[3];
        std::vector<User>::iterator it;
        it = std::find_if(users.begin(),
                          users.end(),
                          [&username](User const& target)
                          {
                            return (target.getName() == username);
                          }
        );

        if(it == users.end()) {
            std::vector<std::string> result;
            parseGroups(command[2], result);
            // TODO(anna): should only be able to add user to groups if those
            // groups already exist
            users.push_back(User(username, result));
        }
        else {
            std::cout << "error: user already exists" << std::endl;
        }
    }
    else {
        if(command.size() != 2) {
            std::cout << "error: must specify user to add" << std::endl;
            return;
        }
        std::string username = command[1];
        std::vector<User>::iterator it;
        it = std::find_if(users.begin(),
                          users.end(),
                          [&username](User const& target)
                          {
                            return (target.getName() == username);
                          }
        );

        if(it == users.end()) {
            users.push_back(User(username));
        }
        else {
            std::cout << "error: user already exists" << std::endl;
        }
    }
}

void Shell::usermod(std::vector<std::string> command) {
    if(command.size() != 4) {
        std::cout << "error: invalid usermod command" << std::endl;
        return;
    }
    if((command[1] == "-a") && (command[2] == "-G")) {
        std::string group_to_add = command[3];
        std::vector<std::string>::iterator it;
        it = std::find_if(s_groups.begin(),
                          s_groups.end(),
                          [&group_to_add](std::string const& target)
                          {
                            return (target == group_to_add);
                          }
        );
        if(it != s_groups.end()) {
            current_user->setGroup(group_to_add);
        }
        else {
            std::cout << "error: group does not exist" << std::endl;
        }
    }
    else {
        std::cout << "error: invalid options" << std::endl;
    }
    return;
}

void Shell::chown(std::vector<std::string> command) {
    if(command.size() != 3) {
        std::cout << "error: must specify new owner and file" << std::endl;
    }
    else {
        Properties* target_prop;
        std::string new_owner = command[1];
        std::string file_to_change = command[2];

        // need to check if new owner exists
        std::vector<User>::iterator it_user;
        it_user = std::find_if(users.begin(),
                            users.end(),
                            [&new_owner](User const& target)
                            {
                                return (target.getName() == new_owner);
                            }
        );

        if(it_user != users.end()) {
            std::vector<File>::iterator it_file;
            it_file = std::find_if(current_dir->files.begin(),
                            current_dir->files.end(),
                            [&file_to_change](File const& target)
                            {
                                return (target.getName() == file_to_change);
                            }
            );

            if(it_file != current_dir->files.end()) {
                target_prop = current_dir->files[std::distance
                    (current_dir->files.begin(), it_file)].getProp();
                target_prop->owner = new_owner;
            }
            else {
                std::cout << "error: file not found" << std::endl;
            }
        }
        else {
            std::cout << "error: user does not exist" << std::endl;
        }
    }

    return;
}

void Shell::chgrp(std::vector<std::string> command) {

    if(command.size() != 3) {
        std::cout << "error: must specify new group and file" << std::endl;
    }
    else {
        Properties* target_prop;
        std::string new_group = command[1];
        std::string file_to_change = command[2];

        // need to check if new group exists
        std::vector<std::string>::iterator it_group;
        it_group = std::find_if(s_groups.begin(),
                            s_groups.end(),
                            [&new_group](std::string const& target)
                            {
                                return (target == new_group);
                            }
        );

        if(it_group != s_groups.end()) {
            std::vector<File>::iterator it_file;
            it_file = std::find_if(current_dir->files.begin(),
                            current_dir->files.end(),
                            [&file_to_change](File const& target)
                            {
                                return (target.getName() == file_to_change);
                            }
            );

            if(it_file != current_dir->files.end()) {
                target_prop = current_dir->files[std::distance
                    (current_dir->files.begin(), it_file)].getProp();
                target_prop->group = new_group;
            }
            else {
                std::cout << "error: file not found" << std::endl;
            }
        }
        else {
            std::cout << "error: group does not exist" << std::endl;
        }
    }

    return;
}

void Shell::execute(std::vector<std::string> command) {
    // command will be one piece, such as "./file"
    if(command.size() != 1) {
        std::cout << "error: invalid command" << std::endl;
        return;
    }
    else {
        // get actual file name to be executed
        std::string file = command[0];
        file = file.substr(2);
        bool found = false;
        for(int i = 0; i < current_dir->files.size(); i++) {
            if((current_dir->files[i].getName() == file) &&
               (current_dir->files[i].getFileType())) {
                found = true;
                break;
            }
        }
        if(found) {
            std::cout << file << " executed" << std::endl;
        }
        else {
            std::cout << "error: file not found" << std::endl;
        }
    }

    return;
}

void Shell::groups(std::vector<std::string> command) {
    // TODO(anna): should this just be for current user? or should you specify
    // username
    if(command.size() == 1) {
        for(int i = 0; i < current_user->u_groups.size(); i++) {
            std::cout << current_user->u_groups[i] << "\t";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "error: unrecognized groups command" << std::endl;
    }
    return;
}

void Shell::groupadd(std::vector<std::string> command) {
    if(command.size() != 2) {
        std::cout << "error: must specify group to add" << std::endl;
        return;
    }
    std::string group_to_add = command[1];
    std::vector<std::string>::iterator it;
    it = std::find_if(s_groups.begin(),
                      s_groups.end(),
                      [&group_to_add](std::string const& target)
                      {
                      return (target == group_to_add);
                      }
    );
    if(it == s_groups.end()) {
        s_groups.push_back(group_to_add);
    }
    else {
        std::cout << "error: group already exists" << std::endl;
    }

    return;
}

void Shell::userdel(std::vector<std::string> command) {
    if(command.size() < 1) {
        std::cout << "error: must specify user and optionally groups" << std::endl;
        return;
    }

    if(command[1] == "-G") {
        if(command.size() != 4) {
            std::cout << "error: must specify groups and username" << std::endl;
            return;
        }
        std::string username = command[3];

        std::vector<User>::iterator it;
        it = std::find_if(users.begin(),
                          users.end(),
                          [&username](User const& target)
                          {
                            return (target.getName() == username);
                          }
        );

        // TODO(anna): should current user only be able to remove
        // their own groups? because that's how this is set up, but without
        // a check enforcing it
        if(it != users.end()) {
            std::string group_to_remove = command[2];
            for(int i = 0; i < current_user->u_groups.size(); i++) {
                if(current_user->u_groups[i] == group_to_remove) {
                    current_user->u_groups.erase(
                        current_user->u_groups.begin()+i);
                }
            }
        }
        else {
            std::cout << "error: nonexistent user: " << username << std::endl;
        }
    }
    else {
        if(command.size() != 2) {
            std::cout << "error: must specify user to delete" << std::endl;
            return;
        }

        std::string username = command[1];
        std::vector<User>::iterator it;
        it = std::find_if(users.begin(),
                          users.end(),
                          [&username](User const& target)
                          {
                            return (target.getName() == username);
                          }
        );
        if(it != users.end()) {
            users.erase(users.begin()+(std::distance(users.begin(), it)));
        }
    }

    return;
}

void Shell::switchto(std::vector<std::string> command) {
    if(command.size() != 2) {
        std::cout << "error: must specify user" << std::endl;
        return;
    }
    else {
        std::string username = command[1];
        std::vector<User>::iterator it;
        it = std::find_if(users.begin(),
                          users.end(),
                          [&username](User const& target)
                          {
                            return (target.getName() == username);
                          }
        );
        if(it != users.end()) {
            User &new_user = *it;
            current_user = &new_user;
        }
        else {
            std::cout << "error: user does not exist" << std::endl;
        }
    }

    return;
}
// Anna Pankiewicz

#include "shell.h"

Shell::Shell() {
    // initialize directory structure and "create" root directory
    root.setName("root");
    root.setFileType(false);
    current_dir = &root;
}

void Shell::process(const std::string& command) {
    return;
}
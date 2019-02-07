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

        void process(const std::string& command);
};

#endif
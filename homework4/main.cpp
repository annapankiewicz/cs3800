// Anna Pankiewicz

#include <iostream>
#include <string>
#include <vector>

#include "file.h"
#include "shell.h"

void prompt(Shell emulator);

int main() {

    Shell shell;
    prompt(shell);

    return 0;
}

void prompt(Shell emulator)
{
    std::string command = "";
    while(!((command == "quit") || (command == "exit")))
    {
        std::cout << "[" << emulator.getCurrentUser() << "@cs3800]$ ";
        getline(std::cin, command);
        emulator.process(command);
    }
}
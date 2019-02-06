// Anna Pankiewicz

#include <iostream>
#include <string>
#include <vector>
#include "file.h"
using namespace std;

void prompt();

int main() {

    // initialize directory structure and "create" root directory
    vector<File> root;
    root.push_back(File ("root", false));

    prompt();

    return 0;
}

void prompt()
{
    string command = "";
    while(!((command == "quit") || (command == "exit")))
    {
        cout << "[anna@cs3800]$ ";
        getline(cin, command);
    }
}
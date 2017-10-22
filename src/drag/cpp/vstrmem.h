#pragma once

#include <sstream>
#include <string>
#include <map>

using std::string;

class ivstream : public std::istringstream
{
        string name;

    public:
        ivstream(string n);
};

class ovstream : public std::ostringstream
{
        string name;
        bool app;

    public:
        ovstream(string n);
        ovstream(string n, std::ios_base::openmode m);
        ~ovstream();
};

extern std::map<string, string> vstrmem;


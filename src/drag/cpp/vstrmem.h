#pragma once

#include <istream>
#include <ostream>
#include <string>

using std::string;

class ivstream : public std::istream
{
    public:
        ivstream(string n);
};

class ovstream : public std::ostream
{
    public:
        ovstream(string n);
        ovstream(string n, ios_base::openmode m);
};

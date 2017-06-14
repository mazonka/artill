#include <iostream>
#include "vstrmem.h"

std::map<string, string> vstrmem;

ivstream::ivstream(string n) : std::istringstream(vstrmem[n])
{
    if ( vstrmem[n].empty() ) throw "Cannot open mem " + n;

    std::cout << "ivstream::ivstream " << n << '\n';
    throw "ivstream";
}

ovstream::ovstream(string n)
{
    std::cout << "ovstream::ovstream " << n << '\n';
    throw "ovstream";
}

ovstream::ovstream(string n, std::ios_base::openmode m)
{
    std::cout << "ovstream::ovstream mode " << n << '\n';
    throw "ovstream-m";
}

ovstream::~ovstream()
{
    std::cout << "ovstream::~ovstream\n";
}



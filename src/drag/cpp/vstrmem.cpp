#include <iostream>
#include "vstrmem.h"

std::map<string, string> vstrmem;

ivstream::ivstream(string n) : std::istringstream(vstrmem[n])
{
    if ( vstrmem[n].empty() ) throw "Cannot open mem " + n;
}

ovstream::ovstream(string n): name(n), app(false)
{
}

ovstream::ovstream(string n, std::ios_base::openmode m): name(n), app(false)
{
    if ( m == std::ios::app ) { app = true; return; }
    std::cout << "ovstream::ovstream bad mode " << n << '\n';
    throw "ovstream " + n + " - bad ios mode";
}

ovstream::~ovstream()
{
    if ( app ) vstrmem[name] += str();
    else vstrmem[name] = str();
}



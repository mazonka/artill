#pragma once
#include <fstream>

#ifndef main

using ivstream = std::ifstream;
using ovstream = std::ofstream;

#else

#include "vstrmem.h"

#endif


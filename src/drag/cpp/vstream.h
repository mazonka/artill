#pragma once
#include <fstream>

#ifndef VSTREAM

using ivstream = std::ifstream;
using ovstream = std::ofstream;

#else

#include "vstream2.h"

#endif


#pragma once

#include <map>

#include "util.h"

struct Names
{
        std::map<string, double *> sd;
        std::map<string, int *> si;
        string filename;

        Names(string file): filename(file) {}
        void reset (string file) { filename = file; }
        void load(int n = 0); // n - check for number of inputs
        void save(string file, int precision = 0);

    private:
        bool recognise(std::pair<string, string> kv);
};


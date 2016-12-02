#include <map>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <iomanip>

#include "util.h"
#include "names.h"

void Names::load(int n)
{
    std::map<string, string> m;

    {
        std::ifstream in(filename.c_str());

        if (!in) throw "Cannot open " + filename;

        string k, v;
        for (; in >> k; )
        {
            if ( k == "#" ) { getline(in, v); continue; }
            in >> v;
            if ( !k.empty() && k[0] != '#' ) m[k] = v;
        }
    }

    int sz = m.size();
    if ( n > 0 && n != sz )
        throw "Not all parameters defined in " + filename
        + ", need " + tos(n) + ", got " + tos(sz);

    while (!m.empty())
    {
        auto kv = *m.begin();
        if ( !recognise(kv) ) throw "Unknown input [" + kv.first + "]";
        m.erase(m.begin());
    }
}

void Names::save(string file, int precision)
{
    std::ofstream of(file.c_str());
    if (!of) throw "Cannot open " + file;

    auto s = [](string x) -> string
    {
        int z = 20 - x.size();
        if (z <= 0) z = 1;
        return string(z, ' ');
    };

    for ( auto x : sd )
    {
        string n = x.first;
        if ( precision > 0 ) of << std::setprecision(precision);
        of << n << s(n) << (*x.second) << '\n';
    }

    for ( auto x : si )
    {
        string n = x.first;
        of << n << s(n) << (*x.second) << '\n';
    }
}

bool Names::recognise(std::pair<string, string> kv)
{
    string k = kv.first;
    string v = kv.second;
    double d = std::atof(v.c_str());
    int i = std::atoi(v.c_str());

    if ( sd.find(k) != sd.end() ) *sd[k] = d;
    else if ( si.find(k) != si.end() ) *si[k] = i;
    else return false;

    return true;
}


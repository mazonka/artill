#pragma once

#include <string>
#include <ostream>
#include <vector>
#include <map>

using std::string;

// defined in vstrmem
extern std::map<string, string> vstrmem;

namespace ut
{
std::vector<int> s2a(const string & s);
string a2s(const std::vector<int> & v);
} // ut

// set io: S text
// get output char: idx
// set input: I text
// run command: R text

class Protocol
{
        int idx;
        const string * text;
        char func; // SIR
        int asize;
        const int * arr;
        string str;

        string mk_str() const;
    public:
        Protocol( char c, const string * t ): text(t), func(c) {}
        Protocol( int sz, const int * a );

        std::vector<int> mk_arr() const;
        char get_f() const { return func; }
        string get_str() const { return str; }

        // input

        // output

        // run

};

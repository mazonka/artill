#include <sstream>
#include <algorithm>

#include "utils.h"

using std::string;
using std::vector;


std::vector<int> ut::s2a(const string & s)
{
    std::vector<int> r;

    for ( size_t i = 0; i < s.size(); i++ )
        r.push_back( (int)(unsigned char)(s[i]) );

    return r;
}

string ut::a2s(const std::vector<int> & v)
{
    string r;

    for ( auto x : v ) r += char(x);

    return r;
}

std::vector<int> Protocol::mk_arr() const
{
    return ut::s2a(func + *text);
}

string Protocol::mk_str() const
{
    std::vector<int> v(arr, arr + asize);
    return ut::a2s(v);
}


Protocol::Protocol( int sz, const int * a ): func('X'), asize(sz), arr(a)
{
    str = mk_str();
    if ( str.empty() ) return;
    func = str[0];
    str = str.substr(1);
}


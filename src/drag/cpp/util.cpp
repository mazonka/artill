#include <cmath>
#include <limits>
#include <cctype>

#include "util.h"

const double PI = std::acos(-1.0);
const double INF_VAL = std::numeric_limits<double>::max();

string format(string q, int SZ)
{
    int z = q.size();
    if ( z < SZ ) q += string(SZ - z, ' ');
    return q;
}

string format(double x, int SZ)
{
    std::ostringstream os; os << x;
    return format(os.str(), SZ);
}

void eatSpaces(string & s)
{
    while ( !s.empty() && std::isspace(s[0]) ) s = s.substr(1);
    while ( !s.empty() && std::isspace(s[s.size() - 1]) ) s = s.substr(0, s.size() - 1);
}

string relpath(string old, string nw)
{
    const char SL = '/';

    if ( old.find(SL) == string::npos ) return nw;

    auto i = old.rfind(SL);

    return old.substr(0, i + 1) + nw;
}


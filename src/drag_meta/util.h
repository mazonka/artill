#pragma once

#include <string>
#include <exception>
#include <sstream>
#include <limits>

#include "defs.h"

using std::string;

extern const double PI;
extern const double INF_VAL;

template<typename T>
inline string tos(T x) { std::ostringstream o; o << x; return o.str(); }

inline double deg2rad(double x) { return x * PI / 180; }
inline double rad2deg(double x) { return x * 180 / PI; }
inline double r2d(double x) { return rad2deg(x); }
inline double rad2mil(double x) { return x * 3200 / PI; }
inline double mil2rad(double x) { return x * PI / 3200; }

inline double linear(double x, double x1, double x2, double y1, double y2)
{ return y1 + (y2 - y1) * (x - x1) / (x2 - x1); }

const int FORMAT_SIZE = 13;
string format(double x, int SZ = FORMAT_SIZE);
string format(string x, int SZ = FORMAT_SIZE);

void eatSpaces(string & s);

template<typename T>
inline bool is_infinite( const T & x )
{
    T max_value = std::numeric_limits<T>::max();
    return x < -max_value || max_value < x;
}

template<typename T>
inline bool is_nan( const T & value ) { return value != value; }

template<typename T>
inline bool is_bad( const T & value ) { return is_infinite(value) || is_nan(value); }

string relpath(string old, string nw);

#define O(x) std::cout<<#x"="<<x<<' ';
#define never(x) throw "bug at "+tos(__FILE__)+":"+tos(__LINE__) + " ["+tos(x)+"]";

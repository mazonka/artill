#include <cstdlib>
#include <cmath>

#include <iostream>
#include <fstream>
#include <vector>

#include "dlibad.h"

using std::cout;

typedef std::vector<double> vd;

///typedef double (*typef)(int, const double *);
double f_orig(int n, const double * x)
{
    return 100 * pow(x[0] + 3, 4) + pow(x[1] - 3, 4);
}

double func(int n, const double * x, const void *)
{
    if ( n % 2 || n < 1 ) throw "Bad vector";

    double k = 1.0 * n * n / 40;

    auto sq = [](double y) -> double { return y * y; };
    //auto len = [&sq](double ax, double ay, double bx, double by) ->
    //           double { return std::sqrt(sq(ax - bx) + sq(ay - by)); };

    auto len2 = [&sq](double ax, double ay, double bx, double by) ->
                double { return (sq(ax - bx) + sq(ay - by)); };

    double r = k * len2(0, 0, x[0], x[1]);
    r += k * len2(x[n - 2], x[n - 1], 1, 0);

    for ( int i = 0; i < n; i += 2 )
    {
        if (i) r += k * len2(x[i - 2], x[i - 1], x[i], x[i + 1]);
        r += x[i + 1];
    }

    if (0)
    {
        std::cout << r << ":";
        for ( int i = 0; i < n; i++ ) std::cout << ' ' << x[i];
        std::cout << "\n";
    }

    if (1)
    {
        static int ctr = 0;
        if ( ++ctr > 100000 )
        {
            ctr = 0;
            std::cout << r << ": " << x[n / 2 - 1] << ' ' << x[n / 2] << '\n';
        }
    }

    return r;
}


vd xmain(const vd & v, Dlibad::typef fn)
{
    Dlibad d(fn, (const void *)nullptr);
    vd r = d.solve(v, 100000);
    return r;
}

int main(int argc, char ** argv)
{
    vd a = { 0.1, 0.1 };
    while ( a.size() / 2 < 11 ) a.push_back(0);

    a = xmain(a, func);
    for ( size_t i = 0; i < a.size(); i++ ) std::cout << ' ' << a[i];
    std::cout << "\n" << func(a.size(), a.data(), nullptr) << '\n';

    std::ofstream of("test.dat");
    of << "0 0\n";
    for ( size_t i = 0; i < a.size(); i += 2 )
        of << a[i] << ' ' << a[i + 1] << '\n';
    of << "1 0\n";
}

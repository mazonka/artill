#include <iostream>

#include <set>

#include "vstream.h"
#include "func.h"

inline void never(int i) { throw std::to_string(i); }

void Function::load(string f)
{
    ivstream in(f.c_str());

    if ( !in ) throw "Cannot open " + f;

    v.clear();

    while (1)
    {
        double x, y;
        in >> x >> y;
        if ( !in ) break;
        v.push_back(Point(x, y));
    }
}

void Function::save(string f)
{
    ovstream of(f.c_str());

    if ( !of ) throw "Cannot open " + f;

    for ( int i = 0; i < size(); i++ )
        of << v[i].x << '\t' << v[i].y << '\n';
}

Function Function::operator-(const Function & f) const
{
    std::set<double> xs;
    for ( int i = 0; i < size(); i++ ) xs.insert(v[i].x);
    for ( int i = 0; i < f.size(); i++ ) xs.insert(f.v[i].x);

    Function r;

    for ( auto x : xs )
    {
        Point p(x, y(x) - f.y(x) );
        r.v.push_back(p);
    }

    return r;
}

double Function::y(double x) const
{
    auto linear = []
                  (double x, double x1, double x2, double y1, double y2)
                  ->double
    { return y1 + (y2 - y1) * (x - x1) / (x2 - x1); };


    int sz = v.size();
    if (sz < 2) never(59);

    if (x < v[0].x ) return 0;
    if (x > v[sz - 1].x ) return 0;
    if (x == v[0].x ) return v[0].y;
    if (x == v[sz - 1].x ) return v[sz - 1].y;

    double z, x1, x2, y1, y2;
    for (int i = 1; i < sz; i++)
    {
        z = v[i].x;
        if (x > z) continue;
        x2 = z;
        x1 = v[i - 1].x;
        y2 = v[i].y;
        y1 = v[i - 1].y;
        return linear(x, x1, x2, y1, y2);
    }

    return 0;
}

double Function::integrate2(int k) const
{
    double sum = 0;
    for ( int i = 1; i < size(); i++ )
    {
        double x1 = v[i - 1].x;
        double x2 = v[i].x;
        double y1 = v[i - 1].y;
        double y2 = v[i].y;
        double d1 = x2 - x1;
        double d = d1;
        for ( int j = 1; j < k; j++ ) d *= d1;
        sum += d * (y1 * y1 + y1 * y2 + y2 * y2);
    }
    return sum / 3;
}

double Function::integrate1() const
{
    double sum = 0;
    for ( int i = 1; i < size(); i++ )
    {
        double x1 = v[i - 1].x;
        double x2 = v[i].x;
        double y1 = v[i - 1].y;
        double y2 = v[i].y;
        sum += (x2 - x1) * (y1 + y2);
    }
    return sum / 2;
}

Function::Function(int n, Point rng)
{
    if ( n < 2 ) throw std::to_string(n) + ": number of points < 2";

    double x1 = rng.x;
    double x2 = rng.y;

    double d = (x2 - x1) / (n - 1);

    for ( int i = 0; i < n - 1; i++ )
        v.push_back(Point(x1 + i * d, 0));

    v.push_back(Point(x2, 0));
}

void Function::sample(const Function & f)
{
    for ( int i = 0; i < size(); i++ )
        v[i].y = f.y(v[i].x);
}

double Function::noise() const
{
    Point r = range();
    double span = r.y - r.x;
    Function f = inject(15);
    return f.fourthder().integrate2(6) / span;
}

Function Function::fourthder() const
{
    const Function & f = *this;
    Function r = f - f;

    std::vector<int> cntr(size(), 0);

    for ( int k = 4; k < size(); k++ ) // select group of 5
    {
        double f44 = 0;
        for ( int ic = 0; ic < 5; ic++ )
        {
            int i = k - 4 + ic;
            double prod = 1;
            for ( int jc = 0; jc < 5; jc++ )
            {
                int j = k - 4 + jc;
                if ( i == j ) continue;
                prod *= (f[i].x - f[j].x);
            }
            f44 += f[i].y / prod;
        }
        f44 *= 24;

        for ( int ic = 0; ic < 5; ic++ )
        {
            int i = k - 4 + ic;
            r.setY( i, r.getY(i) + f44 );
            ++cntr[i];
        }
    }

    for ( int k = 0; k < size(); k++ ) // take average
        if ( cntr[k] ) r.setY( k, r.getY(k) / cntr[k] );

    //r.save("smooth.tmp");
    return r;
}

Function Function::inject(int n) const
{
    Function r;

    const Function & t = *this;

    r.v.push_back(t[0]);

    for (int i = 1; i < size(); i++ )
    {
        Function f(n, Point(t[i - 1].x, t[i].x));
        for (int j = 1; j < f.size(); j++ )
            r.v.push_back(Point(f[j].x, 0));
    }

    for ( int i = 0; i < r.size(); i++ ) r.setY(i, y(r[i].x) );

    return r;
}

// *** non-class members *** //
#include "asolver.h"

void setParamsToFun(Function * pf, const Params & pms)
{
    int sz = pms.v.size();
    for ( int i = 0; i < sz; i++ )
        pf->setY(i, pms.v[i]);
}

void getParamsFromFun(const Function * pf, Params & pms)
{
    int sz = pf->size();
    pms.v = std::vector<double>(sz);
    for ( int i = 0; i < sz; i++ )
        pms.v[i] = pf->getY(i);
}


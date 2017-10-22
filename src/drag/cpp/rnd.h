#pragma once

#include <cmath>
#include <random>
#include <functional>

class Rnd
{
        std::default_random_engine reng;
        std::uniform_real_distribution<double> dist;
        std::function<double()> rnd;

    public:

        static unsigned long seed;

        Rnd(): reng(seed), dist(0, 1), rnd( std::bind(dist, reng) ) {}
        double operator()() { return rnd(); }
};

extern Rnd rnd01;

class Marsaglia
{
        double disp;
        double second;

        static double f(double d, double & t)
        {
            double w1 = 0, w2 = 0, r = 2;

            while ( r >= 1.0 || r < 1e-40 )
            {
                w1 = 2.0 * rnd01() - 1.0;
                w2 = 2.0 * rnd01() - 1.0;
                r = w1 * w1 + w2 * w2;
            }

            r = std::sqrt( -2.0 * std::log(r) / r );
            t = w2 * r * d;
            return ( w1 * r * d );
        }

    public:
        Marsaglia(double d): disp(d), second(0) {}

        double operator()()
        {
            if ( second == 0 ) return f(disp, second);
            double t = second; second = 0; return t;
        }
};

class Marsaglia3d
{
    public:
        double x, y, z;
        Marsaglia3d(double d)
        {
            static const double s3 = std::sqrt(3);
            Marsaglia m(d);
            x = m() / s3;
            y = m() / s3;
            z = m() / s3;
        }
};

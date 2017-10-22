#include <cstdio>
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <functional>


using std::cout;

void tan()
{
    const double R = 6400000;
    double x = R;
    double y = -1.5 * R;
    double result = std::atan2 (x, R + y);
    std::printf ("The arc tangent for (x=%f, y=%f) is %f\n", x, y, result );
}

std::pair<double, double> air_rho(double Z)
{
    Z /= 1000; // to km

    const double P0 = 101325;    // sea level standard pressure, Pa
    const double L = 6.5;        // temperature lapse rate, K/km
    const double E = 6356.766;   // the radius of the earth, km
    const double T0 = 288.15;    // sea level standard temperature, K
    const double g = 9.80665;    // gravitational constant, m/sec2
    const double M = 28.9644;    // molecular weight of dry air, gm/mol
    const double R = 8.31432;    // gas constant, J/(mol*K)
    const double v0 = 20.046;    // speed of sound at 1K


    double H = E * Z / (E + Z);
    double T = T0 - L * H;
    if ( T <= 0 ) return std::make_pair(0., 0.);

    double P = P0 * std::pow(T / T0, g * M / (R * L));

    double rho = P * M / (1000 * R * T);
    double vs = v0 * sqrt(T);
    return std::make_pair(rho, vs);
}

double Cd(double v, double vs)
{
    const double ball_c0 = 0.2, ball_c1 = 0.5,
                 ball_d1 = 0.7, ball_d2 = 1.3, ball_d3 = 3.5;

    const double c0 = ball_c0;
    const double c1 = ball_c1;
    const double d1 = ball_d1;
    const double d2 = ball_d2;
    const double d3 = ball_d3;

    if ( vs < 1e-6 ) return c0;
    double mu = v / vs;

    auto s = [](double x)->double { return 3 * x * x - 2 * x * x * x; };

    if ( mu < d1 ) return c0;
    if ( mu < d2 ) return c0 + (c1 - c0) * s((mu - d1) / (d2 - d1));
    if ( mu < d3 ) return c1 + (c0 - c1) * s((mu - d2) / (d3 - d2));

    return c0;
}

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

unsigned long Rnd::seed = 1;
Rnd rnd01;

class Marsaglia
{
        double disp;
        double second;


        static double f(double d, double & t)
        {
            double x, w1, w2, r = 2;

            while ( r >= 1.0 )
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


int main ()
{
    tan();

    for ( double x = 0; x < 50000; x += 5000 )
    {
        auto a = air_rho(x);
        cout << x << '\t' << a.first << '\t' << a.second << '\n';
    }

    for ( double x = 0; x < 5; x += 1 ) cout << x << '\t' << Cd(x, 1) << '\n';

    int N = 10000;
    double mean = 0, sum2 = 0;
    for ( int i = 0; i < N; i++ )
    {
        double x = Marsaglia(2)();
        mean += x;
        sum2 += x * x;
    }
    mean /= N;
    sum2 /= N;

    cout << "N=" << N << " mean=" << mean << " sum2=" << sum2
         << " sig=" << std::sqrt(sum2 - mean * mean) << '\n';


    double mx = 0, my = 0, mz = 0;
    sum2 = 0;
    for ( int i = 0; i < N; i++ )
    {
        Marsaglia3d w(2);
        sum2 += (w.x * w.x + w.y * w.y + w.z * w.z);
        mx += w.x; my += w.y; mz += w.z;
    }
    mx /= N; my /= N; mz /= N;
    sum2 /= N;

    cout << "N=" << N << " mx,my,mz=" << mx << ' ' << my << ' ' << mz
         << " sum2=" << sum2
         << " sig=" << std::sqrt(sum2 - (mx * mx + my * my + mz * mz)) << '\n';
}


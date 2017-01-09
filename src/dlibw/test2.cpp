#include "dlib/optimization.h"
#include <iostream>


using namespace std;
using namespace dlib;

typedef matrix<double, 0, 1> vec;

// ----------------------------------------------------------------------------------------

class test_function
{
    public:

        test_function (const vec & input) { target = input; }

        double operator() ( const vec & arg) const
        {
            return mean(squared(target - arg));
        }

    private:
        vec target;
};

struct Skewed
{
    double a, b;
    Skewed(): a(30), b(0.2) {}

    double operator() ( const vec & arg) const
    {
        //f(x,y)=1-(1+b*x)*exp(-a*(x**2+y**2-1)**2)
        double x = arg(0);
        auto sq = [](double z) -> double { return z * z; };
        double r2 = 0;
        int sz = arg.size();
        for ( int i = 0; i < sz; i++ ) r2 += sq(arg(i));
        double z = 1 - (1 + b * x) * std::exp(-a * sq(r2 - 1));
        //cout<<"("<<x<<','<<y<<','<<z<<") ";
        for ( int i = 0; i < sz; i++ ) cout << ' ' << arg(i);
        cout << '\n';
        return z;
    }
};

// ----------------------------------------------------------------------------------------

int main()
{
    try
    {
        vec starting_point(4);
        vec target(4);

        //starting_point.set_size(4);

        starting_point = -4, 5, 99, 3;
        target = 3, 5, 1, 7;

        find_min_bobyqa(test_function(target),
                        starting_point,
                        9,    // number of interpolation points
                        uniform_matrix<double>(4, 1, -1e100), // lower bound constraint
                        uniform_matrix<double>(4, 1, 1e100),  // upper bound constraint
                        10,    // initial trust region radius
                        1e-6,  // stopping trust region radius
                        100    // max number of objective function evaluations
                       );
        cout << "test_function solution:\n" << starting_point << endl;


        starting_point.set_size(7);

        double q = -0.5;
        starting_point = q, q, q, q, q, q, q;

        find_min_bobyqa(Skewed(),
                        starting_point,
                        15,    // number of interpolation points
                        uniform_matrix<double>(7, 1, -1e100), // lower bound constraint
                        uniform_matrix<double>(7, 1, 1e100),  // upper bound constraint
                        10,    // initial trust region radius
                        1e-8,  // stopping trust region radius
                        10000    // max number of objective function evaluations
                       );
        cout << "test_function solution:\n" << starting_point << endl;

    }
    catch (std::exception & e)
    {
        cout << e.what() << endl;
    }
}


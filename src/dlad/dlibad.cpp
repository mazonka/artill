#include <iostream>

#include "dlibad.h"

#include "dlib/optimization.h"

typedef dlib::matrix<double, 0, 1> vec;

///int Qmaxeval = 100000;

struct Function
{
    int dim;
    Dlibad::typef fn;
    const void * data;

    Function(int d, Dlibad::typef f, const void * v)
        : dim(d), fn(f), data(v) {}

    double operator() ( const vec & a ) const
    {
        std::vector<double> v(a.begin(), a.end());
        return fn(a.size(), v.data(), data);
    }
};


Dlibad::vd Dlibad::solve(const vd & v, int maxeval) const
{
    int sz = v.size();
    vec starting_point(sz);
    //starting_point.set_size(7);

    for ( int i = 0; i < sz; i++ ) starting_point(i) = v[i];

    try
    {
        find_min_bobyqa(Function(sz, fn, data),
                        starting_point,
                        2 * sz + 1, // number of interpolation points
                        dlib::uniform_matrix<double>(sz, 1, -1e100),
                        dlib::uniform_matrix<double>(sz, 1, 1e100),
                        0.01,    // initial trust region radius
                        1e-12,  // stopping trust region radius
                        maxeval // max number of function evaluations
                       );

    }
    catch (std::exception & e)
    {
        std::cout << "\nmaxeval = " << maxeval;
        std::cout << "\nbobyqa : " << e.what() << '\n';
    }

    return std::vector<double>(starting_point.begin(), starting_point.end());
}


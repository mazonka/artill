
#include "dlibad.h"

#include "dlib/optimization.h"

typedef dlib::matrix<double, 0, 1> vec;

struct Function
{
    int dim;
    Dlibad::typef fn;
    Function(int d, Dlibad::typef f): dim(d), fn(f) {}

    double operator() ( const vec & a ) const
    {
        std::vector<double> v(a.begin(), a.end());
        return fn(a.size(), v.data());
    }
};


Dlibad::vd Dlibad::solve(const vd & v) const
{
    int sz = v.size();
    vec starting_point(sz);
    //starting_point.set_size(7);

    for ( int i = 0; i < sz; i++ ) starting_point(i) = v[i];

    find_min_bobyqa(Function(sz, fn),
                    starting_point,
                    2 * sz + 1, // number of interpolation points
                    dlib::uniform_matrix<double>(sz, 1, -1e100),
                    dlib::uniform_matrix<double>(sz, 1, 1e100),
                    0.01,    // initial trust region radius
                    1e-12,  // stopping trust region radius
                    100000    // max number of objective function evaluations
                   );

    return std::vector<double>(starting_point.begin(), starting_point.end());
}


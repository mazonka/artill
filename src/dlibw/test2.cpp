#include "dlib/optimization.h"
#include <iostream>


using namespace std;
using namespace dlib;

// ----------------------------------------------------------------------------------------

// In dlib, the general purpose solvers optimize functions that take a column
// vector as input and return a double.  So here we make a typedef for a
// variable length column vector of doubles.  This is the type we will use to
// represent the input to our objective functions which we will be minimizing.
typedef matrix<double,0,1> column_vector;

// ----------------------------------------------------------------------------------------

class test_function
{
public:

    test_function (const column_vector& input){ target = input; }

    double operator() ( const column_vector& arg) const
    {
        return mean(squared(target-arg));
    }

private:
    column_vector target;
};

// ----------------------------------------------------------------------------------------

int main()
{
    try
    {
        column_vector starting_point(4);
        column_vector target(4);

        //starting_point.set_size(4);

        starting_point = -4,5,99,3;
        target = 3, 5, 1, 7;

        find_min_bobyqa(test_function(target), 
                        starting_point, 
                        9,    // number of interpolation points
                        uniform_matrix<double>(4,1, -1e100),  // lower bound constraint
                        uniform_matrix<double>(4,1, 1e100),   // upper bound constraint
                        10,    // initial trust region radius
                        1e-6,  // stopping trust region radius
                        100    // max number of objective function evaluations
        );
        cout << "test_function solution:\n" << starting_point << endl;

    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
    }
}


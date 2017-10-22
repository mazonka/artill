#include <cmath>
#include <iostream>

#include "rsolver.h"
#include "vstream.h"

struct Fun : ParabolicSolver::Function
{
    double f(double x) const
    {
        double noise = 0.0001;
        if (noise > 0)
        {
            noise *= (int(1000 * (std::sin(x * 100) + 1)) % 103) / 103.0 - 0.5;
        }

        if (0)
        {
            x /= 45;
            return -x * x + 2 * x + noise;
        }
        else if (1)
        {
            x /= 10;
            return 10 - (3 + x * x) / (1 + x) + noise;
        }

        auto s = [](double x)->double
        {
            double y = (x / 90);
            y *= std::acos(-1);
            y = std::sin(y);
            y *= y * y * y * y;
            return y;
        };
        return s(x) + s(x - 25);
    }
};

void testPS()
{
    Fun fun;
    ParabolicSolver ps(&fun, 0, 80.8957364563456, 10.5);

    double rf = ps.solve(true);

    if ( !ps.zero )
    {
        double rh = ps.solve(false);
        std::cout << "Solved Left  f(" << rf << ")=" << fun.f(rf) << '\n';
        std::cout << "Solved Right f(" << rh << ")=" << fun.f(rh) << '\n';
    }
    else
        std::cout << "Solved Max f(" << rf << ")=" << fun.f(rf) << '\n';

    ovstream of("fun.dat");
    for ( int i = 0; i <= 90; i++ )
        of << i << '\t' << Fun().f(i) << '\n';
}



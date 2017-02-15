#include <cmath>
#include <iostream> // debug
#include <iomanip>

#include "rsolver.h"
#include "traj.h"

Rsolver::RsRes Rsolver::get(int i) const
{
    TrResult trr = Trajectory::run(psi->setAlpha(alpha[i]));
    return RsRes(trr, trt[i]);
}


struct TrajFun : ParabolicSolver::Function
{
    Psi * psi;
    TrajFun(Psi * p): psi(p) {}
    double f(double angle) const
    {
        psi->s.angle_deg = angle;
        TrResult trr = Trajectory::run(psi);
        return trr.range;
    }
};

void Rsolver::init()
{
    alpha[0] = alpha[1] = 0;
    trt[0] = trt[1] = TrType();
}

void Rsolver::solve(double range, TrType typ, double hint)
{
    if (0)
    {
        double x, y;
        Box z(0, 4, 3, 9, 9, 6.0000000001);
        z.apex2(1e-8, x, y);
        std::cout << "apex2: " << std::setprecision(20) << x << ' ' << y << '\n';
        void testPS();
        testPS();
        return;
    }

    TrajFun fun(psi);
    ParabolicSolver ps(&fun, A, B, range);

    if (typ.type == TrType::UNKNOWN || typ.type == TrType::MAX )
    {
        alpha[0] = ps.solve(true, hint);
        trt[0].type = TrType::MAX;
        if (!ps.zero )
        {
            alpha[1] = ps.solve(false, hint);
            trt[0].type = TrType::FLAT;
            trt[1].type = TrType::HIGH;
        }
    }
    else if (typ.type == TrType::FLAT )
    {
        alpha[0] = ps.solve(true, hint);
        trt[0].type = TrType::MAX;
        if (!ps.zero ) trt[0].type = TrType::FLAT;
    }
    else if (typ.type == TrType::HIGH )
    {
        alpha[1] = ps.solve(false, hint);
        trt[1].type = TrType::MAX;
        if (!ps.zero ) trt[1].type = TrType::HIGH;
    }

}


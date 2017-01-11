#include <iostream>
#include <iomanip>
#include <memory>

#include "asolver.h"
#include "dsolver.h"
#include "psic.h"
#include "cd.h"
#include "alglib.h"

using std::cout;

struct DsolvFun : AsolFun
{
        Psi * psi;
        const Dataset * reference;
        Dataset * data, *best;
        double ubest;
        double f(const Params & pms);
        DsolvFun(Psi * p, const Dataset * rf, Dataset * dt)
            : psi(p), reference(rf), data(dt), best(nullptr), ubest(INF_VAL) {}
        ~DsolvFun() { delete best; }

    private:
        DsolvFun(const DsolvFun &);
        void operator=(const DsolvFun &) const;
};

double DsolvFun::f(const Params & pms)
{
    psi->cd->setParams(pms);
    data->run(psi);

    double u = data->util(reference);

    if ( u < ubest )
    {
        //cout << data->dump(reference) << '\n';
        //cout << std::setprecision(17);
        //cout << "{"; for (auto x : pms.v) cout << ' ' << x; cout << " }\n";
        //cout << u << '\n';
        ubest = u;
        delete best;
        best = data->clone();

        psi->cd->saveall();
    }

    static int cntr = 999999;
    //if ( ++cntr > 2 )
    if ( ++cntr > 0 )
    {
        cntr = 0;
        //cout << "min = " << ubest << "     u = " << tos(u) << "    \r" << std::flush;
        cout << u << " : ";
        for ( size_t i = 0; i < pms.v.size(); i++ )
            cout << ' ' << pms.v[i];
        cout << '\n';
    }

    return u;
}

Dsolver::Dsolver(Psi * p, Dataset * d)
    : ref(d), psi(p), data(d->clone())
{
    data->run(psi);
}


void Dsolver::solve()
{
    DsolvFun fun(psi, ref, data);
    Params pms = psi->cd->getParams();
    std::unique_ptr<Asolver> as(make_solver(&fun, pms));
    //Alglib as(&fun, pms);
    //A_dlib as(&fun, pms);

    pms = as->solve();
    psi->cd->setParams(pms);
    delete data;
    data = fun.best->clone();
}


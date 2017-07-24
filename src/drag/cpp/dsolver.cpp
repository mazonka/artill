#include <iostream>
#include <iomanip>
#include <memory>

#include "dlibad.h" // Qmaxeval
#include "asolver.h"
#include "dsolver.h"
#include "psic.h"
#include "cd.h"
#include "func.h"

using std::cout;

struct DsolvFun : AsolFun
{
        Psi * psi;
        const Dataset * reference;
        Dataset * data, *best;
        double ubest;
        double f(const Params & pms, const void * origf);
        DsolvFun(Psi * p, const Dataset * rf, Dataset * dt)
            : psi(p), reference(rf), data(dt), best(nullptr), ubest(INF_VAL) {}
        ~DsolvFun() { delete best; }

    private:
        DsolvFun(const DsolvFun &);
        void operator=(const DsolvFun &) const;
};

double DsolvFun::f(const Params & pms, const void * orig)
{
    psi->cd->setParams(pms);

    Dataset * ds = data->runc(psi);
    double u = ds->util(reference);

    double s = 0;
    bool fempty = true;

    if ( orig)
    {
        const Function * of = static_cast<const Function *>(orig);
        if ( of->size() > 1 )
        {
            Function g(*of, pms.v);
            for ( int i = 0; i < g.size(); i++ ) g.setY(i, pms.v[i]);
            s = g.noise1() + g.noise2() + g.noise4();
            fempty = false;
        }
    }

    double w = u + u * s;

    if ( w < ubest )
    {
        ubest = w;
        delete best;
        best = ds;

        psi->cd->saveall();
        best->save();
    }
    else
        delete ds;

    static int cntr = 1000 * 1000 * 5;
    static int cntrQ = -1; ++cntrQ;
    if ( ++cntr > 9 )
    {
        cntr = 0;
        cout << (Qmaxeval - cntrQ) << " min = " << ubest << "  u = " << tos(u)
             << "  s = " << (fempty ? "nosmooth" : tos(s) )
             <<  "   \r" << std::flush;
        // "nosmooth" means that Function object is not implemented
        // for this Cd, see Cd::buildFunction

        //cout << u << " : "; for ( size_t i = 0; i < pms.v.size(); i++ ) cout << ' ' << pms.v[i]; cout << '\n';
    }

    return w;
}

Dsolver::Dsolver(Psi * p, Dataset * d)
    : ref(d), psi(p), data(d->clone())
{
    data->run_init(psi);
}


void Dsolver::solve()
{
    //Qmaxeval = 100;

    DsolvFun fun(psi, ref, data);
    Params pms = psi->cd->getParams();
    Function orf = psi->cd->buildFunction();
    std::unique_ptr<Asolver> as(make_solver(&fun, pms, &orf));

    pms = as->solve();
    psi->cd->setParams(pms);
    delete data;
    data = fun.best->clone();
}


#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>

#include "dlibad.h" // Qmaxeval
#include "asolver.h"
#include "dsolver.h"
#include "psic.h"
#include "cd.h"
#include "func.h"
#include "vstream.h"
#include "defs.h"

using std::cout;

static int Qcntr = -1;

struct DsolvFun : AsolFun
{
        Psi * psi;
        const Dataset * reference;
        Dataset * data, *best;
        double ubest;
        double smcoeff;
        double f(const Params & pms, const void * origf);
        DsolvFun(Psi * p, const Dataset * rf, Dataset * dt, double smc)
            : psi(p), reference(rf), data(dt), best(nullptr),
              ubest(INF_VAL), smcoeff(smc) {}
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

    double w = u + u * s + smcoeff * s;

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
    ++Qcntr;
    if ( ++cntr > 9 )
    {
        cntr = 0;
        cout << (Qmaxeval - Qcntr) << " min = " << ubest << "  u = " << tos(u)
             << "  s = " << (fempty ? "nosmooth" : tos(s) )
             <<  "   \r" << std::flush;
        // "nosmooth" means that Function object is not implemented
        // for this Cd, see Cd::buildFunction

        //cout << u << " : "; for ( size_t i = 0; i < pms.v.size(); i++ ) cout << ' ' << pms.v[i]; cout << '\n';
    }

    return w;
}

Dsolver::Dsolver(Psi * p, const Dataset * d)
    : ref(d), psi(p), data(d->clone())
{
    data->run_init(psi);
}


void Dsolver::solve(double smcoeff)
{
    Qmaxeval = 10000;
    Qcntr = -1;

    DsolvFun fun(psi, ref, data, smcoeff);
    Params pms = psi->cd->getParams();
    Function orf = psi->cd->buildFunction();
    std::unique_ptr<Asolver> as(make_solver(&fun, pms, &orf));

    pms = as->solve();
    psi->cd->setParams(pms);
    delete data;
    data = fun.best->clone();
}


Vdd load_optim()
{
    Vdd r;

    ivstream in(optim_in);
    ///if ( !in ) return Vdd({Vd({0})});
    if ( !in ) return Vdd();

    for ( string line; (getline(in, line), in); )
    {
        if ( line.empty() ) continue;
        if ( line[0] == '#' ) continue;

        std::istringstream is(line);

        Vd v;
        for ( double x; is >> x; ) v.push_back(x);
        if ( v.empty() ) throw string(optim_in) + ": bad format";
        r.push_back(v);
    }

    return r;
}

double solve_smcoeff(double smcoeff, Psi & psi)
{
    Dataset experiment;
    Dsolver sv(&psi, &experiment);

    cout << std::setprecision(20);

    Dataset * ns = sv.ds()->runc(&psi);
    cout << ns->dump(&experiment) << '\n';
    cout << "Start U=" << ns->util(&experiment) << '\n';
    delete ns;

    sv.solve(smcoeff);

    //cout << sv.ds()->dump(&experiment) << '\n';

    double finalu = sv.u();
    cout << "Final U=" << finalu << '\n';

    sv.ds()->save();
    psi.cd->saveall();
    //Params pms = ds.psi->cd->getParams();
    //cout << "{"; for (auto x : pms.v) cout << ' ' << x; cout << " }\n";

    return finalu;
}

string scenario_str(const Vd & vd)
{
    std::ostringstream os;
    os << "[";
    for ( auto j : vd ) os << ' ' << j;
    os << " ]";
    return os.str();
}

double solve_scenario(const Vd & vd, Params * pms)
{
    Psi psi;
    double finalu = -1;

    for ( auto x : vd )
    {
        std::cout << "\nSolving for smcoeff " << x
                  << " in " << scenario_str(vd) << "\n";

        finalu = solve_smcoeff(x, psi);
        *pms = psi.cd->getParams();
    }

    return finalu;
}

void finalise_solver(Params pms)
{
    Psi psi;
    if( !pms.v.empty() ) psi.cd->setParams(pms);
    solve_smcoeff(0, psi);
}

void strategic_solver()
{
    Vdd scenarios = load_optim();

    double bestu = -1;
    Params bestPms;
    string sc;

    for ( auto & i : scenarios )
    {
        Params p;
        double y = solve_scenario(i, &p);
        if ( bestu < 0 || y < bestu )
        {
            bestu = y;
            bestPms = p;
            sc = scenario_str(i);
        }
    }

    std::cout << "\nFinalising solution for u=" << bestu << " of " << sc << "\n";
    finalise_solver(bestPms);
}


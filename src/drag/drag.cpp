#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <memory>

#include "defs.h"
#include "util.h"
#include "progr.h"
#include "psic.h"
#include "traj.h"
#include "cd.h"
#include "rsolver.h"
#include "dataset.h"
#include "dsolver.h"
#include "alglib.h"
#include "explore.h"

void main_test();
void main_model();
void main_solve();
void main_graph();
void main_shoot(double);
void main_range(double);
void main_maps();

using std::cout;

void usage()
{
    cout << "Usage:\n";
    cout << "\tmodel - compare data sets\n";
    cout << "\tsolve - find best cd\n";
    cout << "\tshoot (angle) - simple shots (default)\n";
    cout << "\trange <R>|max - solve for range\n";
    cout << "\tbuild - continuously improve graph or point\n";
    cout << "\tmaps - explore U function interactively\n";
}

int main(int ac, char * av[])
try
{
    string av1 = "shoot";
    //string av1 = "model";
    //string av1 = "solve";
    //string av1 = "test";
    //string av1 = "graph";
    string av2;
    if ( ac > 1 ) av1 = av[1];
    if ( ac > 2 ) av2 = av[2];

    progress::init(av1);

    if (0);
    else if ( av1 == "test" ) main_test();
    else if ( av1 == "model" ) main_model();
    else if ( av1 == "solve" ) main_solve();
    else if ( av1 == "build" ) main_graph();
    else if ( av1 == "maps" ) main_maps();
    else if ( av1 == "shoot" )
    {
        double r = INF_VAL;
        if ( ac > 2  ) r = std::atof(av2.c_str());
        main_shoot(r);
    }
    else if ( av1 == "range" )
    {
        if ( ac < 3 ) av2 = "max";
        double r = 0;
        if ( av2 == "max" )
        {
            r = INF_VAL;
            cout << "solving for max R\n";
        }
        else
        {
            r = std::atof(av2.c_str());
            cout << "solving for R=" << r << '\n';
        }
        main_range(r);
    }
    else usage();

    progress::end();
}
catch (const char * e)
{
    cout << "Error: " << e << '\n';
    return 1;
}
catch (string e)
{
    cout << "Error: " << e << '\n';
    return 1;
}
catch (std::exception & e)
{
    cout << "Error: " << e.what() << '\n';
    return 2;
}
catch (...)
{
    cout << "Exception caught\n";
    return 3;
}

void main_shoot(double al)
{
    {
        std::ofstream in1(trajs_out);
        std::ofstream in2(earth_out);
        std::ofstream in3(summary_out);

        in3 <<
            "# ElevMils    Range         Z             Y             "
            "TOF           MaxH          Vfin          AOFdeg        "
            "EK0           EKfin         RejectRate%" << '\n';
    }

    Psi psi;

    if ( psi.s.ntraj < 1 ) psi.s.ntraj = 1;

    if ( al == INF_VAL ) {}
    else
    {
        psi.s.angle_deg = al;
        psi.s.ntraj = 1;
    }

    const double & R = cst::Earth_radius;
    const double gacc = cst::Ggrav * cst::Earth_mass / (R * R);
    cout << "Grav g = " << gacc << '\n';

    double last_rng = 0;
    for ( int i = 0; i < psi.s.ntraj; i++ )
    {
        cout << (i + 1) << '/' << psi.s.ntraj << ' ' << std::flush;

        Trajectory t(&psi, i);
        while (t.evaluate());
        TrResult res = t.finalize();

        cout << "saving " << std::flush;
        t.save();
        res.save(!!i);
        cout << "ok\n";

        last_rng = res.range;
        al = res.alpha;
    }

    cout << "Cd use [" << psi.cd->ua << ":" << psi.cd->ub << "]\n";

    cout << std::setprecision(20);
    cout << "Range " << last_rng << " at " << rad2deg(al) << '\n';
}

void main_range(double r)
{
    Psi psi;
    Rsolver rs(&psi, r, TrType());

    Rsolver::RsRes flat = rs.getFlat();
    Rsolver::RsRes high = rs.getHigh();

    const TrResult & f = flat.first;
    const TrResult & h = high.first;

    cout << std::setprecision(20);
    if ( flat.second.type == TrType::MAX )
    {
        cout << "Max range " << f.range << " at " << r2d(f.alpha) << '\n';
    }
    else
    {
        cout << "Range " << f.range << " at "
             << r2d(f.alpha) << " (" << flat.second.str() << ")\n";
        cout << "Range " << h.range << " at "
             << r2d(h.alpha) << " (" << high.second.str() << ")\n";
    }
}

void main_solve()
{
    Psi psi;
    Dataset experiment;
    Dsolver ds(&psi, &experiment);

    cout << std::setprecision(20);

    cout << ds.ds()->dump(&experiment) << '\n';
    cout << "Start U=" << ds.u() << '\n';

    ds.solve();

    cout << ds.ds()->dump(&experiment) << '\n';
    cout << "Final U=" << ds.u() << '\n';

    ds.ds()->save();
    psi.cd->saveall();
    Params pms = ds.psi->cd->getParams();
    cout << "{"; for (auto x : pms.v) cout << ' ' << x; cout << " }\n";
}

void main_model()
{
    Psi psi;
    Dataset experiment;
    Dataset * newds = experiment.clone();

    newds->run0(&psi);
    newds->save();

    cout << newds->dump(&experiment) << " (Cumulative utility E(lnX-lnY)^2)\n";
    delete newds;
}

void main_test()
{
    ///Alglib a;
    std::unique_ptr<Asolver> a(make_solver(nullptr, Params()));
    testAsolver(&*a);
}


void main_graph()
{
    Psi psi;

    int n = psi.cd->sample();

    for (;;)
    {
        Dataset experiment;
        Dsolver ds(&psi, &experiment);
        ds.solve();
        ds.ds()->save();
        psi.cd->saveall();

        progress::msg("finished with " + tos(n));
        progress::msg("\n" + ds.ds()->dump(&experiment) + " (U)\n");

        cout << "Final N=" << n << " u=" << ds.u() << '\n';

        n = psi.cd->sample(2 * n - 1);
    }
}

void main_maps()
{
    Psi psi;
    Dataset experiment;
    Explorer e(&psi, &experiment);

    while (1)
    {
        string line;
        std::cout << "> ";
        std::getline(std::cin, line);
        std::istringstream is(line);
        if ( !e.run(is) ) break;
    }
}


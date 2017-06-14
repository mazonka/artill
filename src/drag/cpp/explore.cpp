#include <iostream>
#include <iomanip>
#include <string>

#include "explore.h"
#include "asolver.h"
#include "cd.h"
#include "vstream.h"

using std::string;
using namespace std;

Explorer::Explorer(Psi * p, Dataset * d)
    : ref(d), psi(p), data(d->clone())
{
    data->run_init(psi);
}


bool Explorer::run(istream & is)
{
    string s;
    is >> s;
    if ( s.empty() ) return true;

    if (0) {}
    else if ( s == "q" || s == "quit" ) return false;
    else if ( s == "h" || s == "help" )
    {
        cout << "q (quit) - to quit\n";
        cout << "h (help) - show help\n";
        cout << "u - run current set of parameter and show U\n";
        cout << "p - show parametrs\n";
        cout << "s N Value - set parametrs, N - index\n";
        cout << "d - show dataset\n";
        cout << "m I1 S1 V1 R1 I2 S2 V2 R2 S2 - generate map (Index Value Range NSteps)\n";
    }
    else if ( s == "u" )
    {
        Dataset * ds = data->runc(psi);
        cout << std::setprecision(20);
        cout << ds->dump(ref);
        cout << '\n';
    }
    else if ( s == "p" )
    {
        std::vector<double> p = psi->cd->getParams().v;

        for ( size_t i = 0; i < p.size(); i++ )
            cout << ' ' << p[i];

        cout << '\n';
    }
    else if ( s == "s" )
    {
        Params p = psi->cd->getParams();

        int index = -1;
        is >> index;
        if ( index < 0 || index >= (int)p.v.size() ) return true;

        double val = 0;
        is >> val;
        p.v[index] = val;
        psi->cd->setParams(p);
    }
    else if ( s == "m" )
    {
        maps(is);
    }
    else if ( s == "d" )
    {
        cout << data->print() << '\n';
    }

    else cout << "? [" << s << "]" << '\n';

    return true;
}

void Explorer::maps(istream & is)
{
    int i1, i2, s1, s2;
    double v1, r1, v2, r2;

    is >> i1;
    if ( !is)
    {
        cout << "loading " << maps_in << "\n";
        ivstream in(maps_in);
        in >> i1 >> s1 >> v1 >> r1 >> i2 >> s2 >> v2 >> r2;
        if ( !in )
        {
            cout << "loading failed\n";
            return;
        }
    }
    else
    {
        is >> s1 >> v1 >> r1 >> i2 >> s2 >> v2 >> r2;

        if ( !is)
        {
            cout << "see help\n";
            return;
        }
    }

    if ( s1 < 3 ) s1 = 1;
    if ( s2 < 3 ) s2 = 1;

    cout << "calculating with\n"
         << '\t' << i1 << ' ' << s1 << ' ' << v1 << ' ' << r1 << '\n'
         << '\t' << i2 << ' ' << s2 << ' ' << v2 << ' ' << r2 << '\n';

    Params p0 = psi->cd->getParams();

    if ( i1 < 0 || i1 >= (int)p0.v.size()
            || i2 < 0 || i2 >= (int)p0.v.size() )
    {
        cout << "index out of bouds\n";
        return;
    }

    ovstream of(maps_out);

    cout << "0%\r" << std::flush;

    for ( int i = 0; i < s1; i++ )
    {
        for ( int j = 0; j < s2; j++ )
        {
            Params p = p0;
            double x = s1 > 1 ? v1 - r1 + i * 2 * r1 / (s1 - 1) : v1;
            double y = s2 > 1 ? v2 - r2 + j * 2 * r2 / (s2 - 1) : v2;
            p.v[i1] = x;
            p.v[i2] = y;

            psi->cd->setParams(p);
            Dataset * ds = data->runc(psi);
            double u = ds->util(ref);

            of << std::setprecision(15);
            of << x << '\t';
            of << y << '\t';
            of << u << '\n';
            //cout<<x<<' '<<y<<'\n';
        }

        of << '\n';

        double pg = 100.0 * (i + 1) / s1;
        cout << (int(pg + 0.5)) << "%\r" << std::flush;
    }

    cout << "100%\n";

    psi->cd->setParams(p0);
}


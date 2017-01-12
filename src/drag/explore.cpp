#include <iostream>
#include <iomanip>
#include <string>

#include "explore.h"
#include "asolver.h"
#include "cd.h"

using std::string;
using namespace std;

Explorer::Explorer(Psi * p, Dataset * d)
    : ref(d), psi(p), data(d->clone())
{
    data->run0(psi);
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
    else if ( s == "d" )
    {
        cout << data->print() << '\n';
    }

    else cout << "? [" << s << "]" << '\n';

    return true;
}

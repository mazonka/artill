#include <iostream>
#include <cstring>

#include "workclass_api.h"
#include "utils.h"

#define IFACEDECL
#include "wc_thin_api.h"


const bool DB = false;
using std::cout;

void hello(const void * a, int sz)
{
    const char * s = static_cast<const char *>(a);
    if ( a ) for ( int i = 0; i < sz; i++ ) std::cout << s[i];
    std::cout << "Function 'hello' (void*,int) args\n";
}

void hi()
{
    std::cout << "Simple function 'hi'\n";
}


WorkClass * togs(intptr_t igs)
{
    void * p = reinterpret_cast<void *>(igs);
    WorkClass * gp = static_cast<WorkClass *>(p);
    return gp;
}

int WC_Delete(intptr_t i)
{
    if ( i <= 0 ) return 1;

    if (DB) cout << "WC_Delete: " << i << '\n';
    delete togs(i);
    return 0;
}

intptr_t WC_Create()
{
    void * p = new WorkClass;
    intptr_t x = reinterpret_cast<intptr_t>(p);
    if (DB) cout << "WC_Create: " << x << '\n';
    return x;
}

double WC_GetElement(intptr_t gs, int x)
{
    return 60;
}

int WC_Calculate(intptr_t gs, int x)
{
    const string & n = togs(gs)->getio();
    const string & s = vstrmem[n];
    if ( x >= (int)s.size() ) return 0;
    if ( x < 0 ) return 0;
    return int((unsigned char)(s[x]));
}

int WC_SendData(intptr_t gs, int nx, int ny, const int * x, const double * y)
{
    Protocol p(nx, x);

    switch (p.get_f())
    {
        case 'S': togs(gs)->setio(p.get_str()); break;
        case 'I': togs(gs)->set_input(p.get_str()); break;
        case 'R': togs(gs)->run(p.get_str()); break;
        default: return 77;
    }

    return 0;
}



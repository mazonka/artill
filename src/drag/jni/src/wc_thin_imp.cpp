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


int WC_Calculate(intptr_t gs, int x)
{
    return togs(gs)->calculate(x);
}

double WC_GetElement(intptr_t gs, int x)
{
    return togs(gs)->getElement(x);
}

int WC_SendData(intptr_t gs, int nx, int ny, const int * x, const double * y)
{
    Protocol p(nx, x);
    cout << "AAA " << p.get_f() << ' ' << p.get_str() << '\n';
    return togs(gs)->sendData(nx, ny, x, y);
}



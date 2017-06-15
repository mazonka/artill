#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <cmath>

#include "workclass_api.h"
#include "wc_thin_api.h"
#include "utils.h"

using std::cout;
using std::string;

WorkClass::WorkClass()
{
    dyn_reference = WC_Create();
}

WorkClass::~WorkClass()
{
    WC_Delete(dyn_reference);
}


double WorkClass::getElement(int x)
{
    return -1;
}

int WorkClass::calculate(int x)
{
    return WC_Calculate(dyn_reference, x);
}

int WorkClass::sendData(int nx, int ny, const int * x, const double * y)
{
    return 0;
}

static int senddata(const Protocol & p, intptr_t dr)
{
    auto x = p.mk_arr();

    int k = WC_SendData(dr, (int)x.size(), 0, x.data(), 0);

    if ( k ) throw "Excepton in drag library";

    return 0;
}

void WorkClass::setio(const string & name)
{
    senddata(Protocol('S', &name), dyn_reference);
}

void WorkClass::set_input(const string & data)
{
    senddata(Protocol('I', &data), dyn_reference);
}

string WorkClass::get_output()
{
    string r;

    for ( int i = 0; ; i++ )
    {
        int x = WC_Calculate(dyn_reference, i);
        if ( x == 0 ) break;
        r += (char)x;
    }

    return r;
}

int WorkClass::run(const string & cmd)
{
    return senddata(Protocol('R', &cmd), dyn_reference);
}

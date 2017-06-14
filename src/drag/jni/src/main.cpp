#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

#include "apiclass_iface.h"
#include "apiclass.h"

void testdll()
{
    const char a[] = "hi\n";
    hello(a,sizeof(a)-1);
    hello(0,0);
    hi();
}

using std::cout;

int main(int ac, char *av[])
{
    testdll();

    intptr_t gs = AC_Create();

    int k = AC_Calculate(gs,123);


    double p0 = AC_GetElement(gs,0);

    std::vector<int> x;
    std::vector<double> y;

    x.push_back(123);
    y.push_back(124);

    AC_SendData(gs,x.size(),y.size(),x.data(),y.data());

    AC_Delete(gs);
}



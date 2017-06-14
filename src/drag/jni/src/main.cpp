#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

#include "workclass_api.h"

using std::cout;

int main(int ac, char * av[])
try
{
    WorkClass x;
    x.run("test");
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

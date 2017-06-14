#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <cmath>

#include "workclass_api.h"
#include "utils.h"

using std::cout;
using std::string;

WorkClass::WorkClass()
{
}

WorkClass::~WorkClass()
{
}


double WorkClass::getElement(int x)
{
    return -1;
}

int WorkClass::calculate(int x)
{
    cout << "WorkClass::calculate " << x << '\n';
    return x + 1;
}

int WorkClass::sendData(int nx, int ny, const int * x, const double * y)
{
    return 0;
}

void WorkClass::test()
{
	void main_test();
	main_test();
}

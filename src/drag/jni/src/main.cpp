#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

#include "workclass_api.h"

using std::cout;

int main(int ac, char *av[])
{
	WorkClass x;
	int z = x.calculate(122);
	cout<<z<<'\n';
}


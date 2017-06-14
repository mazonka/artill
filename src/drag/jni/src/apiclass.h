#include <string>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <cmath>

using std::string;

class ApiClass
{
public:
	ApiClass();
	~ApiClass();

	int calculate(int x);
	double getElement(int x);
	int sendData(int nx, int ny, const int *x, const double *y);
};

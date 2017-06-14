#include <string>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <cmath>

using std::string;

class WorkClass
{
        intptr_t dyn_reference;

        WorkClass(const WorkClass &) = delete;
        void operator=(const WorkClass & x) = delete;

    public:
        WorkClass();
        ~WorkClass();

        int calculate(int x);
        double getElement(int x);
        int sendData(int nx, int ny, const int * x, const double * y);

        void test();
};

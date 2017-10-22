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
        string ioname;

        WorkClass(const WorkClass &) = delete;
        void operator=(const WorkClass & x) = delete;

    public:
        WorkClass();
        ~WorkClass();

        // legacy functions
        int calculate(int x);
        double getElement(int x);
        int sendData(int nx, int ny, const int * x, const double * y);

        // access member
        string getio() const { return ioname; }

        // dynamic (non-virtual) functions
        void setio(const string & name);
        void set_input(const string & data);
        string get_output();
        int run(const string & cmd);
};

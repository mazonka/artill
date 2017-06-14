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

void WorkClass::input(const string &name, const string &data){}

string WorkClass::output(const string &name){ return "XX"; }

int WorkClass::run(const string &cmd)
{
  std::istringstream is(cmd);
  std::vector<string> vs;
  for( string s; is>>s; ) vs.push_back(s);

  std::cout<<vs.size()<<' '<<vs[0]<<'\n';

  int sz = (int)vs.size();
  std::vector<const char *> av;

  for( int i=0; i<sz; i++ ) av.push_back(vs[i].data());

  int dragmain(int ac, const char **av);
  int k = dragmain(sz,av.data());

  return k;
}



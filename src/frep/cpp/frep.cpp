#include <iostream>
#include <memory>
#include <exception>
#include <sstream>

#include "frep.h"
#include "func.h"
#include "asolver.h"

using std::cout;

bool options(int ac, char * av[], Function & f_in, Function & f_p, string & out);
void buildFun(const Function * a, Function * b);
void normalise(const Function * a, Function * b, bool doit);

int main(int ac, char * av[])
try
{
    //void test01(); test01();
    //void test02(); test02();
    //void test03(); test03();
    //void test04(); test04();

    Function a, b;
    string out;

    if ( !options(ac, av, a, b, out) ) return 0;
    buildFun(&a, &b);
    normalise(&a, &b, false);

    b.save(out);
}
catch (string e)
{
    cout << "Error: " << e << '\n';
    return 2;
}
catch (std::exception & e)
{
    cout << "Errro (C++ exception) : " << e.what() << '\n';
    return 2;
}
catch (...)
{
    cout << "Exception";
    return 1;
}


bool options(int ac, char * av[], Function & f_in, Function & f_p, string & out)
{
    if ( ac < 3 )
    {
        cout << "Usage: input_file number [ output_file ]\n";
        cout << "Or: input_file file_points [ output_file ]\n";
        return false;
    }

    string inp = av[1];
    f_in.load(inp);
    cout << "Input file : " << inp << '\n';

    {
        std::istringstream is(av[2]);
        int x; is >> x;
        if (is.fail()) // name
        {
            f_p.load(av[2]);
            cout << "Parametrising with " << f_p.size()
                 << " points from " << av[2] << '\n';
        }
        else
        {
            f_p = Function(x, f_in.range());
            cout << "Parametrising with " << f_p.size()
                 << " points, same range\n";
        }
    }

    out = inp + ".out";
    if ( ac > 3 ) out = av[3];
    cout << "Output file : " << out << '\n';

    return true;
}

// START TESTS

void test01()
{
    Function a;
    a.load("tests/t01a.dat");
    Function b("tests/t01b.dat");

    Function c = a - b;

    c.save("tests/t01c_o.dat");
}

void test02()
{
    Function a("tests/t02a.dat");
    cout << "t02a integrate2 => " << a.integrate2() << '\n';
    cout << "t02a integrate1 => " << a.integrate1() << '\n';
}

void test03()
{
    Function a("tests/t03a.dat");
    Function b("tests/t03b.dat");

    b.sample(a);

    Function c(3, a.range());
    c.sample(a);

    b.save("tests/t03b_o.dat");
    c.save("tests/t03c_o.dat");
}

// TEST 04

struct Fun : AsolFun
{
    const Function * bf; // base
    Function * pf;

    Fun(const Function * a, Function * b): bf(a), pf(b) {}

    double f(const Params & pms);
};

double Fun::f(const Params & pms)
{
    setParamsToFun(pf, pms);
    double r = (*bf - *pf).integrate2();

    static double r0 = 1e100; if( r0>r ){ cout<<"   (" <<r<<")      \r"; r0=r; }

    return r;
}

void test04()
{
    Function a("tests/t04a.dat");
    Function b(3, a.range());
    b.sample(a);
    b.save("tests/t04b1_o.dat");
    b.setY(1, 1.2);
    b.save("tests/t04b2_o.dat");

    Fun fun(&a, &b);
    Params pms;
    ///pms.v.push_back(0);
    ///pms.v = {0, 0, 0};
    ///pms.v = std::vector(3,0);
    getParamsFromFun(&b, pms);

    std::unique_ptr<Asolver> as(make_solver(&fun, pms));
    pms = as->solve();

    cout << "Solved : {";
    for ( auto x : pms.v ) cout << ' ' << x;
    cout <<  " }\n";

    setParamsToFun(&b, pms);
    b.save("tests/t04b3_o.dat");

    cout << "a => " << a.integrate1() << '\n';
    cout << "b => " << b.integrate1() << '\n';
}

// END TESTS

void buildFun(const Function * a, Function * b)
{
    b->sample(*a);

    Fun fun(a, b);
    Params pms;
    getParamsFromFun(b, pms);

    std::unique_ptr<Asolver> as(make_solver(&fun, pms));
    pms = as->solve();

    cout << "Solved : {"; for ( auto x : pms.v ) cout << ' ' << x; cout <<  " }\n";

    setParamsToFun(b, pms);
}

void normalise(const Function * a, Function * b, bool doit)
{
    double sa = a->integrate1();
    double sb = b->integrate1();

    cout << "a => " << sa << '\n';
    cout << "b => " << sb << '\n';

	if( !doit ) return;

    // a = b + z*dx => z = (a-b)/dx

    int sz = b->size();
    Point r = b->range();
    double z = (sa - sb) / (r.y-r.x);

    for ( int i = 0; i < sz; i++ )
        b->setY(i, b->getY(i) + z);

    cout << "b => " << b->integrate1() << '\n';
}

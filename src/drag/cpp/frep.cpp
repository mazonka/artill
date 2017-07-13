#include <iostream>
#include <memory>
#include <exception>
#include <sstream>
#include <limits>
#include <set>

#include "frep.h"
#include "func.h"
#include "asolver.h"
#include "util.h"

using std::cout;

bool options(int ac, const char * av[],
             Function & f_in, Function & f_p, string & out, bool & reduce);

void buildFun(const Function * a, Function * b);
void normalise(const Function * a, Function * b, bool doit);
void reduceFun(const Function * a, Function * b);

int main_frep(int ac, const char * av[])
try
{
    //void test01(); test01();
    //void test02(); test02();
    //void test03(); test03();
    //void test04(); test04();

    Function a, b;
    string out;

    bool reduce = false;
    if ( !options(ac, av, a, b, out, reduce) ) return 0;

    if ( reduce )
    {
        reduceFun(&a, &b);
    }
    else
    {
        buildFun(&a, &b);
        normalise(&a, &b, false);
    }

    b.save(out);

    cout << "noise1 a = " << a.noise1() << '\n';
    cout << "noise1 b = " << b.noise1() << '\n';
    cout << "noise2 a = " << a.noise2() << '\n';
    cout << "noise2 b = " << b.noise2() << '\n';
    cout << "noise4 a = " << a.noise4() << '\n';
    cout << "noise4 b = " << b.noise4() << '\n';

    return 0;
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


bool options(int ac, const char * av[],
             Function & f_in, Function & f_p, string & out, bool & reduce)
{
    if ( ac < 3 )
    {
        cout << "Usage:  input_file  number       output_file\n";
        cout << "or:     input_file  file_points  output_file\n";
        cout << "or:     input_file  @number      output_file\n\n";
        cout << "number defines number of points for reparametrisation\n";
        cout << "@number defines number of points to gracefully reduce\n\n";
        return false;
    }

    string inp = av[1];
    f_in.load(inp);
    cout << "Input file : " << inp << '\n';

    string av2 = av[2];
    if ( av[2][0] == '@' )
    {
        reduce = true;
        av2 = av2.substr(1);
    }


    {
        std::istringstream is(av2);
        int x; is >> x;
        if (is.fail()) // name
        {
            if ( reduce ) throw "Use number with @";
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

    out = av[3];
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
    cout << "t02a integrate2 => " << a.integrate2(1) << '\n';
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

    double f(const Params & pms, const void *);
};

double Fun::f(const Params & pms, const void *)
{
    setParamsToFun(pf, pms);
    double r = (*bf - *pf).integrate2(1);

    static double r0 = 1e100; if ( r0 > r )
    { cout << "   (" << r << ")      \r"; r0 = r; }

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

    std::unique_ptr<Asolver> as(make_solver(&fun, pms, nullptr));
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

    std::unique_ptr<Asolver> as(make_solver(&fun, pms, nullptr));
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

    if ( !doit ) return;

    // a = b + z*dx => z = (a-b)/dx

    int sz = b->size();
    Point r = b->range();
    double z = (sa - sb) / (r.y - r.x);

    for ( int i = 0; i < sz; i++ )
        b->setY(i, b->getY(i) + z);

    cout << "b => " << b->integrate1() << '\n';
}

Function dropInFun(const Function & c, const std::set<int> & i)
{
    std::vector<double> xs, ys;
    for ( int j = 0; j < c.size(); j++ )
    {
        if ( i.find(j) != i.end() ) continue;
        xs.push_back(c[j].x);
        ys.push_back(c[j].y);
    }

    return Function(Function(xs), ys);
}

void reduceFun(const Function * a, Function * b)
{
    if ( b->size() < 2 ) throw "no < 2";
    Function c = *a;
    while ( c.size() > b->size() )
    {
        std::set<int> todel;
        double mn = std::numeric_limits<double>::max();

        for ( int i = 1; i < c.size() - 1; i++ )
        {
            Function d = c;
            std::set<int> ex; ex.insert(i);
            d = dropInFun(d, ex);

            double z = (*a - d).integrate2();

            if ( z > mn ) continue;
            if ( z < mn ) { todel.clear(); mn = z; }
            todel.insert(i);
        }

        if ( todel.empty() ) never("");

        c = dropInFun(c, todel);

        std::cout << c.size() << "  \r";
    }
    std::cout << '\n';

    *b = c;
}


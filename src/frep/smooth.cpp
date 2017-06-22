#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct F
{
    std::vector<double> vx, vy;
    void add(double x, double y) { vx.push_back(x); vy.push_back(y); }
    int size() const { return (int)vx.size(); }
};


double noise(const F &f);

int main()
{
    F f;
    ifstream in("smooth.dat");
    while (in)
    {
        double x, y;
        in >> x >> y;
        if ( !in ) break;
        f.add(x, y);
    }
    cout<<"loaded "<<f.size()<<" points\n";

    cout<<"Noise: "<<noise(f);
}


double noise(const F &f)
{
	F p = fourthder(f);
	return int2(p);
}


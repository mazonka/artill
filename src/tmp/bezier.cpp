#include <vector>
#include <iostream>
#include <fstream>

using std::vector;
using std::cout;

class Binomial
{
		typedef double basetype;
        typedef vector<basetype> vu;
        vector<vu> tbl;

        basetype & v(int n, int k)
        {
            while ( n >= (int) tbl.size() ) tbl.push_back(vu());
            vu & v = tbl[n];
            while ( k >= (int) v.size() ) v.push_back(0);
            return v[k];
        }

    public:

        basetype operator()(int n, int k)
        {
            if (n < k) return 0;
            if (k == 0 || n == 1 ) return 1;
            if (n == 2 && k == 1) return 2;
            if (n == 2 && k == 2) return 1;
            if (n == k) return 1;

            if (v(n,k)) return v(n,k);
            v(n,k) = (*this)(n - 1, k) + (*this)(n - 1, k - 1);
			//cout<<"set n,k,B : "<<n<<' '<<k<<' '<<v(n,k)<<'\n';
            return v(n,k);
        }

		int fill(int m)
		{
			for( int i=0; i<m; i++ )
			for( int j=0; j<m; j++ ) (*this)(i,j);
			return m;
		}
};

Binomial bin;

inline double linear(double x, double x1, double x2, double y1, double y2)
{ return y1 + (y2 - y1) * (x - x1) / (x2 - x1); }

double calc(const vector<double> &v, double x)
{
    auto p = [](double x)->double { return x < 0 ? 0 : x; };

    int sz = v.size();
    if (sz < 2) throw (0);
    if (sz % 2) throw (0);

	double xn = v[0];
	double xx = v[sz-2];

    if (x <= xn ) return p(v[1]);
    if (x >= xx ) return p(v[sz-1]);
	int n = sz/2-1; // number of segments
	double seg = (xx-xn)/n;

	//double t = (xn-x)/(xx-xn); this is not right - it gives equal weights
    double z, x1, x2, y1, y2, t=0;
	for( int i=0; i<sz; i += 2 )
	{
        z = v[i];
        if (x > z) continue;
        x2 = z;
        x1 = v[i - 2];
        //y2 = v[i + 1];
        //y1 = v[i - 1];
		double t0 = (i/2-1)*seg;
        t = p(linear(x, x1, x2, t0, t0+seg));
	}

    double sum=0, vx, vy;
    for (int i = 0; i <= n; i++ )
    {
        //vx = v[2*i];
        vy = v[2*i + 1];
		double t1 = std::pow(1-t,n-i);
		double t2 = std::pow(t,i);
		sum += bin(n,i)*t1*t2*vy;
    }

    return p(sum);
}


int main()
{
///cout<<"AAA\n";
	bin.fill(100);

	vector<double> graph;

	{	
		std::ifstream in("input");
		if( !in ){ cout<<"input\n"; return 1; }
		for( double x; in>>x; ) graph.push_back(x);
	}

	std::ofstream of("out.dat");

	for( double x=0; x<4; x+=0.1 )
	of<<x<<'\t'<<calc(graph,x)<<'\n';
}

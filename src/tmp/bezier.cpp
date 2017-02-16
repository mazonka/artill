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

double calc(const vector<double> &v, double x)
{
	return 0;
}


int main()
{
	Binomial bin;
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

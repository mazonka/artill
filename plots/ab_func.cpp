#include <cmath>
#include <iostream>

using namespace std;

typedef double R;

const R Pi = acos(-1.0);

R Exp(R x){ return exp(x); }
R Sqrt(R x){ return sqrt(x); }
R Erf(R x){ return erf(x); }
R Power(R x, R y){ return pow(x,y); }

double fB(R x, R v, R t)
{
    x -= v;
    return exp(-x * x / (t * t));
}

double fB0(R v, R t)
{
return
(t*(t/Exp( Power(v, 2)/Power(t, 2)) + Sqrt(Pi)*v + 
          Sqrt(Pi)*v*Erf(v/t)))/2.;
}

double fB1(R v, R t)
{
return
(t*((2*t*v)/Exp( Power(v, 2)/Power(t, 2)) + 
          Sqrt(Pi)*(Power(t, 2) + 2*Power(v, 2)) + 
          Sqrt(Pi)*(Power(t, 2) + 2*Power(v, 2))*Erf(v/t)))/4.;
}

double fB2(R v, R t)
{
return
(t*(2*Power(t, 3) + 
          3*Exp( Power(v, 2)/Power(t, 2))*Sqrt(Pi)*Power(t, 2)*v + 
          2*t*Power(v, 2) + 
          2*Exp( Power(v, 2)/Power(t, 2))*Sqrt(Pi)*Power(v, 3) + 
          Exp( Power(v, 2)/Power(t, 2))*Sqrt(Pi)*
            v*(3*Power(t, 2) + 2*Power(v, 2))*Erf(v/t)))/(4.*
      Exp( Power(v, 2)/Power(t, 2)));
}

R T(R v){ return 1+v*v; }

R DP1(R v)
{
	return fB1(v,T(0)) -v*fB0(v,T(0))+ fB1(0,T(v))*fB0(v,T(0))/fB0(0,T(v));
}

R DP(R v)
{
	return DP1(v);//-DP1(-v);
}

#define O(x) cout<<' '<<(#x)<<'='<<x;
int main()
{
	//O(fB0(0,1)) O(fB0(2,1)) O(fB1(0,1)) O(fB1(2,1)) return 0;

    R A = 0, B = 2;
    int sz = 200;
    for ( int i = 1; i < sz; i++ )
    {
        R v = A + (B - A) * i / (sz - 1);
        cout << v << '\t' << DP(v)  
		<< '\t' << fB1(v,T(0))
		//<< '\t' << fB1(0,T(v))
		<< '\t' << fB0(v,T(0))
		//<< '\t' << fB0(0,T(v))
		<< '\n';
    }
}

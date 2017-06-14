#include <iostream>
#include <cstring>

#include "apiclass.h"

#define IFACEDECL
#include "apiclass_iface.h"


const bool DB = false;
using std::cout;

void hello(const void* a, int sz)
{
	const char * s = static_cast<const char*>(a);
	if( a ) for( int i=0; i<sz; i++ ) std::cout<<s[i];
	std::cout<<"Function 'hello' (void*,int) args\n";
}

void hi()
{
	std::cout<<"Simple function 'hi'\n";
}


ApiClass * togs(intptr_t igs)
{
	void * p = reinterpret_cast<void*>(igs);
	ApiClass * gp = static_cast<ApiClass*>(p);
	return gp;
}

int AC_Delete(intptr_t i)
{
	if( i<=0 ) return 1;

	if(DB) cout<<"AC_Delete: "<<i<<'\n';
	delete togs(i);
	return 0;
}

intptr_t AC_Create()
{
	void * p = new ApiClass;
	intptr_t x = reinterpret_cast<intptr_t>(p);
	if(DB) cout<<"AC_Create: "<<x<<'\n';
	return x;
}


int AC_Calculate(intptr_t gs, int x)
{
	return togs(gs)->calculate(x);
}

double AC_GetElement(intptr_t gs, int x)
{
	return togs(gs)->getElement(x);
}

int AC_SendData(intptr_t gs, int nx, int ny, const int * x, const double * y)
{
	return togs(gs)->sendData(nx,ny,x,y);
}



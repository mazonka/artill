#include <cstdint>

#ifndef IFACEDECL
#define IFACEDECL
#else
#undef IFACEDECL
#define IFACEDECL __declspec(dllexport)
#endif

extern "C" void IFACEDECL hello(const void *, int sz);
extern "C" void IFACEDECL hi();

extern "C"
{

int IFACEDECL AC_Delete(intptr_t gs);
intptr_t IFACEDECL AC_Create();

int IFACEDECL AC_Calculate(intptr_t gs, int x);
double IFACEDECL AC_GetElement(intptr_t gs, int x);
int IFACEDECL AC_SendData(intptr_t gs, int nx, int ny, const int * x, const double * y);

} // C



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

    int IFACEDECL WC_Delete(intptr_t gs);
    intptr_t IFACEDECL WC_Create();

    int IFACEDECL WC_Calculate(intptr_t gs, int x);
    double IFACEDECL WC_GetElement(intptr_t gs, int x);
    int IFACEDECL WC_SendData(intptr_t gs, int nx, int ny, const int * x, const double * y);

} // C



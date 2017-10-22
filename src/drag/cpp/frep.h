#pragma once

#include "func.h"
#include "asolver.h"

void setParamsToFun(Function * pf, const Params & pms);
void getParamsFromFun(const Function * pf, Params & pms);

int main_frep(int ac, const char * av[]);


#include <fstream>

#include "defs.h"
#include "timer.h"
#include "progr.h"


void progress::init(string f)
{
    std::ofstream of(progr_out);

    if ( !of )
        throw string() + "Output directory is not accessible: " + progr_out;

    of << Timer::getHms() << " started " << f << '\n';
}

void progress::msg(string m)
{
    std::ofstream of(progr_out, std::ios::app);
    of << Timer::getHms() << " " << m << "\n";
}

void progress::end()
{
    msg("done");
}

#pragma once

#include "psic.h"
#include "dataset.h"

class Explorer
{
        const Dataset * ref;

        Psi * psi;
        Dataset * data;

        double u() const { return data->util(ref); }
        void maps(std::istream & is);

    public:
        ~Explorer() { delete data; }
        Explorer(Psi * p, Dataset * d);

        bool run(std::istream & is);

    private:
        Explorer(const Explorer &);
        void operator=(const Explorer &);
};

#pragma once

#include <vector>

#include "psic.h"
#include "fpoint.h"
#include "trres.h"

class Trajectory
{
        const Psi * psi;
        std::vector<Fpoint> path;
        double dt;
        double dEacc;
        double trlength;
        int reject;

        int check;
        int renorm;

        double Ek0, Ep0;
        double angle0;

        void cutEnd();
        void smooth();
        void fillResult(TrResult & res);
		double mach(Fpoint p);

    public:

        Trajectory(const Psi * psi, int idlt);
        bool evaluate();
        void save();
        TrResult finalize();

        static TrResult run(const Psi * psi);
};



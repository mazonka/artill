#pragma once

#include <vector>

#include "psic.h"
#include "trres.h"

// There are three types of Entries
// ANGLE: (v) alpha      ... ~R ~Max -> vatiate nothing
// RANGE: (v) (alpha) R  ... ~Max    -> variate alpha
// MAX:   v (alpha) Max  ... ~R      -> variate alpha

class Entr2
{
		bool isdef;
public:
        static Entr2 * create(const string & line, const Entr2 * def);
        bool isDef() const { return isdef;  }
};

class Dmeta;

class Ds2
{
        std::vector<Entr2 *> entries;

        void load(string file);

    public:
        Ds2();
        ~Ds2();

        Ds2 run(const Dmeta & m, const Psi & psi) const;
        void save() const;
        string dump(const Ds2 & ref) const;

    private:
        Ds2(const Ds2 &);
        void operator=(const Ds2 &);
};

class Dmeta
{
    public:
        Dmeta(const Ds2 & ds, const Psi & psi);
};


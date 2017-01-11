#include <fstream>
#include <iostream> //debug
#include <cctype>
#include <typeinfo>
#include <cmath>
#include <iomanip>

#include "defs.h"
#include "util.h"
#include "rsolver.h"
#include "ds2.h"
#include "trres.h"

Ds2::Ds2()
{
    load(dataset_in);
}

void Ds2::load(string file)
{
    std::ifstream in(file.c_str());
    if ( !in ) throw "Cannot open " + file;

    Entr2 * curdef = nullptr;

    bool comment = false;

    for (int ln = 1; in; ln++)
    {
        string line;
        std::getline(in, line);
        if ( !in ) break;

        eatSpaces(line);
        if ( line.empty() ) continue;
        if ( line[0] == '#' ) continue;
        if ( line[0] == '@' )
        {
            string newfile = relpath(file, line.substr(1));
            load(newfile);
            continue;
        }

        if ( line[0] == '{' ) { comment = true; continue; }
        if ( comment )
        {
            if ( line[0] == '}' ) comment = false;
            continue;
        }

        Entr2 * en;
        try { en = Entr2::create(line, curdef); }
        catch (string e)
        {
            throw  "Reading " + file + " line " + tos(ln) + " " + e;
        }

        if ( en->isDef() )
        {
            std::getline(in, line); ++ln;
            eatSpaces(line);

            if ( line.empty() || line[0] != '-' )
                throw "Reading " + file + " line " + tos(ln)
                + " entry definition must be underlined";

            delete curdef;
            curdef = en;
            continue;
        }

        entries.push_back(en);
    }
}

Ds2 run(const Dmeta & m, const Psi & psi) const;
//void Dataset::run(Psi * psi)
{
	Ds2 nds(0);
    for (auto entry : entries)
        entry->run(m,psi);
}

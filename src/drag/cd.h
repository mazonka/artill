#pragma once

#include <vector>
#include <cstring>

#include "names.h"
#include "asolver.h"

class Cd
{
        virtual double calc(double x) = 0;

    public:
        double ua, ub; // usage range

        static Cd * factory(int);
        Cd(): ua(-1), ub(-1) {}
        virtual ~Cd() {}

        double calc(double v, double c);
        virtual Cd * clone() const = 0;
        virtual Params getParams() = 0;
        virtual void setParams(const Params &) = 0;

        void saveall() { savecd(); saveplt(); }
        void saveplt(const char [] = cdgrplt_out);
        virtual void savecd() = 0;

        virtual int sample(int = 0) = 0; // takes new size, returns new size
};

class CdLiner : public Cd
{
        Names nm;
        struct B { double c0, c1, c2, d1, d2, d3, shape; };
        union
        {
            B b;
            double p[7];
        } b;

        void setnm();

    public:
        CdLiner();
        CdLiner(const CdLiner &);
        double calc(double x);
        void savecd();
        CdLiner * clone() const { return new CdLiner(*this); }
        Params getParams() { return Params(7, b.p); }

        void setParams(const Params & v)
        { std::memcpy(b.p, v.data(), 7 * sizeof(double)); }

        int sample(int) { throw "CdLiner: sample not supported"; }
};

class CdGraph : public Cd
{
        double a, b; // range [a:b]
        std::vector<double> v;

    public:
        CdGraph();
        double calc(double x);
        void savecd();
        CdGraph * clone() const { return new CdGraph(*this); }
        Params getParams() { return Params(v.size(), v.data()); }
        void setParams(const Params &);
        int sample(int);
};

class CdPoint : public Cd
{
        std::vector<double> v;

    public:
        CdPoint();
        double calc(double x);
        void savecd();
        CdPoint * clone() const { return new CdPoint(*this); }
        Params getParams() { return Params(v.size() - 1, v.data() + 1); }
        void setParams(const Params &);
        int sample(int);
};

// same as Point but holds differences in X, not absolute Xs
class CdPointD : public Cd
{
        std::vector<double> v;

    public:
        CdPointD();
        double calc(double x);
        void savecd();
        CdPointD * clone() const { return new CdPointD(*this); }
        Params getParams() { return Params(v.size() - 1, v.data() + 1); }
        void setParams(const Params &);
        int sample(int);
};

class CdAlpha : public Cd
{
        Names nm;
        struct B
        {
            double a0, a1, am, aw, b1, bm, bw;
            double calc(double x);
        };
        union
        {
            B b;
            double p[7];
        } b;

        void setnm();

    public:
        CdAlpha();
        CdAlpha(const CdAlpha &);
        double calc(double x) { return b.b.calc(x); }
        void savecd();
        CdAlpha * clone() const { return new CdAlpha(*this); }
        Params getParams() { return Params(7, b.p); }

        void setParams(const Params & v)
        { std::memcpy(b.p, v.data(), 7 * sizeof(double)); }

        int sample(int) { throw "CdAlpha: sample not supported"; }

        void savecd(const char * file);
        void loadcd(const char * file);
};


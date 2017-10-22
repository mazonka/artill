#pragma once

#include <deque>
#include <utility>

struct Box2
{
    double a, b;
    double fa, fb;
    Box2(double x1, double x2, double y1, double y2)
        : a(x1), b(x2), fa(y1), fb(y2) {}
    Box2() {}
};

struct Box
{
    double a, b, c;
    double fa, fb, fc;

    Box(double x1, double x2, double x3, double y1, double y2, double y3)
        : a(x1), b(x2), c(x3), fa(y1), fb(y2), fc(y3) {}
    Box() {}

    double D() const { return -(a - b) * (a - c) * (b - c); }

    double A() const
    {
        return  -(c * (fb - fa) + b * (fa - fc)
                  + a * (fc - fb));
    }

    double B() const
    {
        return  -(c * c * (fa - fb)
                  + b * b * (fc - fa) + a * a * (fb - fc));
    }

    double C() const
    {
        return  -(b * c * (b - c) * fa + c * a * (c - a) * fb
                  + a * b * (a - b) * fc);
    }

    bool apex(double err, double & x);
    bool apex2(double err, double & x, double & y);
    bool root(double err, double level, double & x1, double & x2);
};

// finds maximum of 1D function
class ParabolicSolver
{
    public:
        struct Function
        {
            virtual double f(double x) const = 0;
        };

        static const int SCALE = 10;

    private:

        const Function * fun;
        double ra, rb, level;

    public:
        bool zero;

        ParabolicSolver(const Function * f, double a, double b, double y)
            : fun(f), ra(a), rb(b), level(y), zero(false) {}

        double triangle1(Box & x, bool flat); // original oleg's
        double triangle2(Box & x, bool flat); // parabolic optimizator

        double solve(Box2 & x);

        double solve(bool flat);
        double solve(bool flat, double hint);
};


// Least Squares Regression for Quadratic Curve Fitting
class Lsrqcf
{
        static const double SMLL;
        static const size_t vmax = 5;
        double a0, b0, level;
        std::deque<double> vx, vy;
        bool apx_ok;
        double apx, det; // apex and determinant
        double a, b, c, d;
        double S00, S10, S20, S30, S40, S01, S11, S21;
        void add(double x, double y);
        void remove();
        bool calc(bool init);

    public:
        Lsrqcf(Box b, double lev);

        std::pair<bool, double> apex() const
        { return std::pair<bool, double>(apx_ok, apx); }

        double detr() const { return det; }
        bool next(double x, double y);
        void range(double & ax, double & bx) const;
        bool isApexIn() const;
};

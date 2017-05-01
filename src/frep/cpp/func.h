#pragma once

#include <vector>
#include <string>

using std::string;

struct Point
{
    double x, y;
    Point(double a, double b): x(a), y(b) {}
};

class Function
{
        std::vector<Point> v;

    public:
        int size() const { return (int)v.size(); }

        Function() {}
        Function(string f) { load(f); }
        Function(int n, Point rng);

        void load(string f);
        Function operator-(const Function & f) const;
        void save(string f);

        double y(double x) const;
        double integrate2() const;
        double integrate1() const;

        void sample(const Function & f);

        void setY(int i, double y) { v[i].y = y; }
        double getY(int i) const { return v[i].y; }
	Point range() const { return Point(v[0].x,v[size() - 1].x); }
};



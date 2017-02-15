#pragma once

#include <ostream>

struct Fpoint
{
    double t, qx, qy, qz, vx, vy, vz;
    mutable double r1, r2;

    Fpoint(double a, double b, double c, double d, double e)
        : t(a), qx(b), qy(c), qz(0), vx(d), vy(e), vz(0), r1(-1) {}

    double R1() const { if (r1 < 0) calcR(); return r1; }
    double R2() const { if (r1 < 0) calcR(); return r2; }
    void calcR() const;

    Fpoint next(double h) const;

    static Fpoint f(Fpoint a);

    Fpoint operator*=(double d)
    {
        qx *= d; qy *= d; qz *= d; t *= d;
        vx *= d; vy *= d; vz *= d; r1 = -1;
        return *this;
    }

    Fpoint operator+=(const Fpoint & a)
    {
        t += a.t;
        qx += a.qx; qy += a.qy; qz += a.qz;
        vx += a.vx; vy += a.vy; vz += a.vz;
        r1 = -1;
        return *this;
    }

    friend std::ostream & operator<<(std::ostream & o, Fpoint a)
    {
        return o << "{" << a.t << ','
               << a.qx << ',' << a.qy << ',' << a.qz << ','
               << a.vx << ',' << a.vy << ',' << a.vz << '}';
    }

    bool operator==(const Fpoint & a) const
    {
        return qx == a.qx && qy == a.qy && qz == a.qz
               && vx == a.vx && vy == a.vy && vz == a.vz;
    }
};

inline Fpoint operator*(double d, Fpoint a) { return a *= d; }
inline Fpoint operator+(Fpoint a, const Fpoint & b) { return a += b; }

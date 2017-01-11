#include <cmath>
#include <iostream> // debug
#include <iomanip>
#include <vector>

#include "util.h"
#include "psolver.h"

const double Lsrqcf::SMLL = 1e-15;

bool Box::apex(double err, double & x)
{
    double qb = B();
    double qa = A();

    if ( qa < -err )
    {
        x = -0.5 * qb / qa;
        return true;
    }

    return false;
}

bool Box::apex2(double err, double & x, double & y)
{
    if ( !apex(err, x) ) return false;

    double dd = D();

    if ( std::abs(dd) < err ) return false;

    y = (A() * x * x + B() * x + C()) / dd;

    return true;
}

bool Box::root(double err, double level, double & x1, double & x2)
{
    double qb = B();
    double qa = A();
    double cdy = (C() - D() * level);
    double Det = qb * qb - 4 * qa * cdy;

    if ( Det < 0 ) Det = 0;
    Det = std::sqrt(Det);

    if ( a < -err )
    {
        x2 = 0.5 * (-qb + Det) / qa;
        x1 = 0.5 * (-qb - Det) / qa;
        return true;
    }

    if ( std::abs(b) > err )
    {
        x1 = x2 = -cdy / qb;
        return true;
    }

    return false;
}

double ParabolicSolver::solve(bool flat)
{
    double d = (rb - ra) / SCALE;

    std::vector<double> v; v.reserve(SCALE + 1);

    int mx = 0;
    for ( int i = 0; i <= SCALE; i++ )
    {
        double s = ra + i * d;
        double f = fun->f(s);
        v.push_back(f);

        if ( v[i] > v[mx] )
            mx = i;
    }

    if ( level < v[0] || level < v[SCALE] )
        throw "solution " + tos(level) + " outside of ("
        + tos(v[0]) + ":" + tos(v[SCALE]) + ")"
        " - solver can handle only ballistic trajectories";

    if ( mx == 0 || mx == SCALE )
        never("trajectories");

    if ( level >= v[mx] )
    {
        double s = ra + mx * d;
        Box b(s - d, s, s + d, v[mx - 1], v[mx], v[mx + 1]);
        return triangle2(b, flat);
    }

    Box2 bx;

    if (flat)
    {
        for ( int i = 1; i <= mx; i++ )
            if ( v[i] <= level ) continue;
            else
            {
                double s = ra + i * d;
                bx = Box2(s - d, s, v[i - 1], v[i]);
                break;
            }

    }
    else
    {
        for ( int i = SCALE; i >= mx; i-- )
            if ( v[i] <= level ) continue;
            else
            {
                double s = ra + i * d;
                bx = Box2(s, s + d, v[i], v[i + 1]);
                break;
            }

    }

    double r = solve(bx);

    return r;
}

double ParabolicSolver::triangle1(Box & x, bool flat)
{
    double y = 0;

    for (int ctr = 1; ; ctr++)
    {
        if ( level <= x.fb ) break;
        if (x.fa == x.fc && x.fb <= x.fa )
        {
            zero = true;
            return x.a;
        }

        double ab = (x.b - x.a) / SCALE;
        double bc = (x.c - x.b) / SCALE;
        bool left = (ab > bc);

        if ( x.apex(1e-15, y) )
        {
            if ( left )
            {
                if ( y > x.b - ab ) y = x.b - ab;
                if ( y < x.a + ab ) y = x.a + ab;
            }
            else // right
            {
                if ( y > x.c - bc ) y = x.c - bc;
                if ( y < x.b + bc ) y = x.b + bc;
            }
        }
        else
        {
            if ( left ) y = (x.b + x.a) / 2;
            else y = (x.c + x.b) / 2;
        }

        double fy = fun->f(y);

        if (fy < x.fa || fy < x.fc) // reached precision limit
        {
            zero = true;
            return x.b;
        }

        if ( left && fy >= x.fb ) x = Box(x.a, y, x.b, x.fa, fy, x.fb);
        if ( left && fy <  x.fb ) x = Box(y, x.b, x.c, fy, x.fb, x.fc);
        if ( !left && fy <  x.fb ) x = Box(x.a, x.b, y, x.fa, x.fb, fy);
        if ( !left && fy >= x.fb ) x = Box(x.b, y, x.c, x.fb, fy, x.fc);

        if (x.fa == x.fb && x.fa > x.fc)
        {
            y = (x.b + x.a) / 2;
            fy = fun->f(y);
            x = Box(x.a, y, x.b, x.fa, fy, x.fb);
        }
        else if (x.fc == x.fb && x.fa < x.fb)
        {
            y = (x.b + x.c) / 2;
            fy = fun->f(y);
            x = Box(x.b, y, x.c, x.fb, fy, x.fc);
        }

        std::cout << ctr << ' ' << std::setprecision(20) << x.a << ' ' << x.b << ' ' << x.c << '\n';

        if ( level > fy ) continue;
    }

    Box2 bf(x.a, x.b, x.fa, x.fb);
    Box2 bh(x.b, x.c, x.fb, x.fc);

    double rf;
    if (flat) rf = solve(bf);
    else rf = solve(bh);

    return rf;
}

double ParabolicSolver::triangle2(Box & x, bool flat)
{
    const double smll = 1e-8;

    if ( x.b - x.a < smll || x.c - x.b < smll )
        never("input 1");
    if ( x.fc > x.fb || x.fa >= x.fb )
        never("input 2");

    Lsrqcf q(x, level);

    double y = x.b, fy;

    if (!q.apex().first)
    {
        if (x.apex(smll, y))
            q.next(y, fun->f(y));
    }

    while ( 1 )
    {
        auto k = q.apex();
        if ( !k.first ) break;
        y = k.second;
        fy = fun->f(y);

        if ( !q.next(y, fy) || !q.isApexIn()) break;

        if (level <= fy) goto levels;
    }

    zero = true;
    return y;

levels:

    double rf;
    if (flat)
    {
        Box2 bf(ra, y, fun->f(ra), fy);
        rf = solve(bf);
    }
    else
    {
        Box2 bh(y, rb, fy, fun->f(rb));
        rf = solve(bh);
    }

    return rf;
}

Lsrqcf::Lsrqcf(Box bx, double lv)
    : a0(bx.a), b0(bx.c), level(lv), apx_ok(false), apx(0), det(0)
{
    S00 = S10 = S20 = S30 = S40 = S01 = S11 = S21 = 0;

    add(bx.a, bx.fa);
    add(bx.b, bx.fb);
    add(bx.c, bx.fc);

    calc(true);
}

bool Lsrqcf::calc(bool init)
{
    auto sq = [](double x)->double { return x * x; };
    auto cu = [](double x)->double { return x * x * x; };

    double zd = (S00 * S20 * S40 - sq(S10) * S40
                 - S00 * sq(S30) + 2 * S10 * S20 * S30 - cu(S20));
    double za = (S01 * S10 * S30 - S11 * S00 * S30 - S01 * sq(S20)
                 + S11 * S10 * S20 + S21 * S00 * S20 - S21 * sq(S10));
    double zb = (S11 * S00 * S40 - S01 * S10 * S40 + S01 * S20 * S30
                 - S21 * S00 * S30 - S11 * sq(S20) + S21 * S10 * S20);
    double zc = (S01 * S20 * S40 - S11 * S10 * S40 - S01 * sq(S30)
                 + S11 * S20 * S30 + S21 * S10 * S30 - S21 * sq(S20));

    if (!init)
    {
        double zz = za * zd;
        if (zz >= 0)
        {
            apx_ok = false;
            return false;
        }
        zz = -za / zd; // real a
    }

    d = zd;
    a = za;
    b = zb;
    c = zc;

    if (std::abs(a) > SMLL)
    {
        apx = -0.5 * b / a;
        det = apx * apx - (c - level * d) / a;
        if (det < 0) det = 0;
        det = std::sqrt(det);
        apx_ok = true;
    }
    else
        apx_ok = false;

    return true;
}

void Lsrqcf::add(double x, double y)
{
    vx.push_back(x); vy.push_back(y);

    double x2 = x * x;
    double x3 = x2 * x;

    S00 += 1;
    S10 += x;
    S20 += x2;
    S30 += x3;
    S40 += x2 * x2;
    S01 += y;
    S11 += x * y;
    S21 += x2 * y;
}

void Lsrqcf::remove()
{
    double x = vx[0];
    double y = vy[0];
    vx.pop_front();
    vy.pop_front();

    double x2 = x * x;
    double x3 = x2 * x;

    S00 -= 1;
    S10 -= x;
    S20 -= x2;
    S30 -= x3;
    S40 -= x2 * x2;
    S01 -= y;
    S11 -= x * y;
    S21 -= x2 * y;
}

bool Lsrqcf::next(double x, double y)
{
    add(x, y);

    if (vx.size() > vmax)
        remove();

    return calc(false);
}

void Lsrqcf::range(double & mn, double & mx) const
{
    mn = vx[0], mx = vx[0];
    for (auto x : vx)
    {
        if (x > mx) mx = x;
        if (x < mn) mn = x;
    }
}

bool Lsrqcf::isApexIn() const
{
    double wa, wb;
    range(wa, wb);
    if (apx < 2 * wa - wb)
        return false;
    if (apx > 2 * wb - wa)
        return false;
    return true;
}

double ParabolicSolver::solve(Box2 & b)
{
    if (level == b.fa) return b.a;
    if (level == b.fb) return b.b;
    double fmax = b.fa > b.fb ? b.fa : b.fb;
    double fmin = b.fa < b.fb ? b.fa : b.fb;
    double xmax = b.fa > b.fb ? b.a : b.b;
    double xmin = b.fa < b.fb ? b.a : b.b;

    auto ln = [&](double z)->double
    {
        return xmin + (xmax - xmin) * (z - fmin) / (fmax - fmin);
    };

    auto cu = [&](double z)->double
    {
        // k * 4 * (x - 0.5)**3 + 0.5;
        double k = (z - fmin) / (fmax - fmin);
        double k3 = k - 0.5;
        k3 = k3 * k3 * k3;
        k = 4 * k3 + 0.5;
        return xmin + (xmax - xmin) * k;
    };

    if (level < fmin || level > fmax)
        never("Box2 input");

    double x = cu(level);
    double fx = fun->f(x);


    while (1)
    {
        Lsrqcf q(Box(xmin, x, xmax, fmin, fx, fmax), level);

        auto k = q.apex();
        if ( k.first )
        {
            double xq = k.second - q.detr();
            if (b.a > b.b) xq += q.detr();

            if (xq >= xmin && xq <= xmax)
            {
                x = xq;
                fx = fun->f(x);
            }
        }

        if (fx == level) return x;

        if (fx < level && fx > fmin)
        {
            fmin = fx;  xmin = x;
        }
        else if (fx > level && fx < fmax)
        {
            fmax = fx; xmax = x;
        }
        else break;

        x = cu(level);
        fx = fun->f(x);
    }

    x = ln(level);

    return x;
}

int code(bool right, double y1, double y2, double lev, bool flat)
{
    int ret = 0;
    if (y1 < y2)
    {
        if (y2 < lev) ret = 123;
        else if (y1 < lev) ret = 132;
        else ret = 312;
    }
    else // y2>=y1
    {
        if (y1 < lev) ret = 213;
        else if (y2 < lev) ret = 231;
        else ret = 321;
    }

    ret *= 10; if (flat) ret++;
    ret *= 10; if (right) ret++;
    return ret;
}

double ParabolicSolver::solve(bool flat, double hint)
{
    if ( hint < ra || hint > rb ) return solve(flat);

    const double stp0 = 1e-6;
    const double base = 10;
    double step = stp0;

    double x1 = hint;
    double y1 = fun->f(x1);
    if (y1 == level) return x1;

    double x0 = x1, y0 = y1;

nxt:
    double x2 = x1 + step;

    if ( (step > 0 && x2 > rb) || (step < 0 && x2 < ra) )
        return solve(flat);

    double y2 = fun->f(x2);
    if (y2 == level) return x2;

    // test house
    if (( (x0 > x1 && x1 > x2 ) || ( x0 < x1 && x1 < x2) )
            && y0 < y1 && y2 < y1)
    {
        Box b1(x0, x1, x2, y0, y1, y2);
        Box b2(x2, x1, x0, y2, y1, y0);
        if (x0 < x2) return triangle2(b1, flat);
        else return triangle2(b2, flat);
    }

    int c = code(step > 0, y1, y2, level, flat);

    auto next = [&x1, &x2, &y1, &y2, &x0, &y0]()
    { x0 = x1; y0 = y1; x1 = x2; y1 = y2; };

    //     123  132  213  231  312  321
    // 00  nxt  got  bck  bck  trn  bck
    // 01  nxt  nxt  trn  got  nxt  nxt
    // 10  nxt  nxt  trn  got  nxt  nxt
    // 11  nxt  got  bck  bck  trn  bck

    if (0);
    else if (c == 12300 || c == 12301 || c == 12310 || c == 12311
             || c == 13201 || c == 13210 || c == 31201 || c == 31210
             || c == 32101 || c == 32110 )
    {
        next();
        step *= base;
        goto nxt;
    }
    else if (c == 13200 || c == 13211 || c == 23101 || c == 23110 )
    {
        Box2 b1(x1, x2, y1, y2);
        Box2 b2(x2, x1, y2, y1);
        if (x1 < x2) return solve(b1);
        else return solve(b2);
    }
    else if (c == 21300 || c == 21311 || c == 23100 || c == 23111
             || c == 32100 || c == 32111 )
    {
        step *= -base;
        goto nxt;
    }
    else if (c == 21310 || c == 21301 || c == 31200 || c == 31211 )
    {
        step = -step;
        goto nxt;
    }
    else never("code " + tos(c));

    never("psolve");
}


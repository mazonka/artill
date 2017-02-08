#pragma once

#include <vector>

#include "psic.h"
#include "trres.h"

// There are three types of Entries
// ANGLE: (v) alpha      ... ~R ~Max -> variate nothing
// RANGE: (v) (alpha) R  ... ~Max    -> variate alpha
// MAX:   v (alpha) Max  ... ~R      -> variate alpha

struct Item
{
    double x;

    Item() : x(0) {}
    virtual ~Item() {}
    static Item * create(string name);
    virtual Item * clone() const = 0;
    virtual void set(const TrSolution & tr) = 0;
    virtual string nm() const = 0;
    virtual double util(const Item * d) const; // hint and kind redefine

    bool isUnknown() const;
    virtual double convert(bool load, double z) { return z; }
};

class Entry
{
        std::vector<Item *> items;
        enum Type { DEF, ANG, RNG, MAX } type;

        void figureOutType();
        void run_max(Psi * psi);
        void run_ang(Psi * psi);
        void run_rng(Psi * psi);
        Item * find(Item * p) const;

    public:
        static Entry * create(const string & line, const Entry * def);
        Entry * clone() const;
        virtual ~Entry();
        bool isDef() const { return type == DEF;  }
        template < class T> Item * find() const { T t; return find(&t); }
        void run(Psi * psi);
        bool sameDef(const Entry * q) const;
        string str(bool head) const;
        double util(const Entry * d) const;

        string getKindStr() const;
        string getListStr(int sz) const;
        string getTypeStr() const;
        double getHint() const;
        void setHint(double);
        double getRange() const;

        void restore(const Entry * e);
};

class Dataset
{
        std::vector<Entry *> entries;

        void load(string file);
        void run_inplace(Psi * psi);

    public:
        Dataset();
        Dataset(int) {} // for cloning
        ~Dataset();
        Dataset * clone() const;

        void run_init(Psi * psi);

        Dataset * runc(Psi * psi) const
        { Dataset * d = clone(); d->run_inplace(psi); return d; }

        void save() const;
        double util(const Dataset * d, std::vector<double> * v = nullptr) const;

        std::vector<Entry *> getEntries() const { return entries; }
        string dump(const Dataset * ref) const;
        string print() const;

    private:
        Dataset(const Dataset &);
        void operator=(const Dataset &);
};

namespace item
{
struct Unknown : Item
{
    string snm;
    static string name() { return "unknown"; }
    Unknown(string name) : Item(), snm(name) {}
    Unknown * clone() const { return new Unknown(*this); }
    void set(const TrSolution & tr) {}
    string nm() const { return snm; }
};

struct Velocity : Item
{
    static string name() { return "V"; }
    Velocity * clone() const { return new Velocity(*this); }
    void set(const TrSolution & tr) { x = tr.r.v0;  }
    string nm() const { return name(); }
};

struct Max : Item
{
    static string name() { return "Max"; }
    Max * clone() const { return new Max(*this); }
    void set(const TrSolution & tr) { x = tr.r.range;  }
    string nm() const { return name(); }
};

struct Range: Item
{
    static string name() { return "R"; }
    Range * clone() const { return new Range(*this); }
    void set(const TrSolution & tr) { x = tr.r.range; }
    string nm() const { return name(); }
};

struct Angle: Item
{
    enum Unit { DEG, MIL, RAD } units;
    static string name_deg() { return "Angle"; }
    static string name_mil() { return "AngMil"; }
    static string name_rad() { return "AngRad"; }
    Angle * clone() const { return new Angle(*this); }
    void set(const TrSolution & tr) { x = r2d(tr.r.alpha); }
    string nm() const;
    Angle(Unit u = DEG): units(u) {}
    virtual double convert(bool load, double x);
};

struct Fall: Item
{
    enum Unit { DEG, MIL, RAD } units;
    static string name_deg() { return "Fall"; }
    static string name_mil() { return "FallMil"; }
    static string name_rad() { return "FallRad"; }
    ///static string name() { return "Fall"; }
    Fall * clone() const { return new Fall(*this); }
    void set(const TrSolution & tr) { x = r2d(tr.r.beta); }
    string nm() const;
    Fall(Unit u = DEG): units(u) {}
    virtual double convert(bool load, double x);
};

struct TopH: Item
{
    static string name() { return "topH"; }
    TopH * clone() const { return new TopH(*this); }
    void set(const TrSolution & tr) { x = tr.r.topHeight; }
    string nm() const { return name(); }
};

struct Drop: Item
{
    static string name() { return "Drop"; }
    Drop * clone() const { return new Drop(*this); }
    void set(const TrSolution & tr) { x = tr.r.drop; }
    string nm() const { return name(); }
};

struct TopR: Item
{
    static string name() { return "topR"; }
    TopR * clone() const { return new TopR(*this); }
    void set(const TrSolution & tr) { x = tr.r.topRange; }
    string nm() const { return name(); }
};

struct Time: Item
{
    static string name() { return "Time"; }
    Time * clone() const { return new Time(*this); }
    void set(const TrSolution & tr) { x = tr.r.tim; }
    string nm() const { return name(); }
};

struct Vfin: Item
{
    static string name() { return "Vf"; }
    Vfin * clone() const { return new Vfin(*this); }
    void set(const TrSolution & tr) { x = tr.r.vf; }
    string nm() const { return name(); }
};

struct Kind: Item
{
    Kind() { x = TrType().dbl(); }
    static string name() { return "Kind"; }
    Kind * clone() const { return new Kind(*this); }
    void set(const TrSolution & tr) { x = tr.t.dbl(); }
    string nm() const { return name(); }
    double util(const Item * d) const { return 0; }
};

struct Hint: Item
{
    Hint() { x = INF_VAL; }
    static string name() { return "Hint"; }
    Hint * clone() const { return new Hint(*this); }
    void set(const TrSolution & tr) { x = tr.hint; }
    string nm() const { return name(); }
    double util(const Item * d) const { return 0; }
};


} // item

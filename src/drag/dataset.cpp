#include <fstream>
#include <iostream> //debug
#include <cctype>
#include <typeinfo>
#include <cmath>
#include <iomanip>

#include "defs.h"
#include "util.h"
#include "rsolver.h"
#include "dataset.h"
#include "trres.h"

Entry::~Entry() { for (auto p : items) delete p; }
Dataset::~Dataset() { for (auto p : entries) delete p; }


//Angle TopHeight
//---------------
//10    1000

Dataset::Dataset()
{
    load(dataset_in);
}

void Dataset::load(string file)
{
    std::ifstream in(file.c_str());
    if ( !in ) throw "Cannot open " + file;

    Entry * curdef = nullptr;

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

        Entry * en;
        try { en = Entry::create(line, curdef); }
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

Entry * Entry::create(const string & line, const Entry * def)
{
    Entry * en = nullptr;

    if (std::isalpha(line[0]))
    {
        en = new Entry();
        std::istringstream is(line);
        for (string s; is >> s;)
        {
            Item * it = Item::create(s);
            if ( en->find(it) )
                throw s + " already defined";
            en->items.push_back(it);
        }
        en->figureOutType(); // validate only
        en->type = DEF;
        return en;
    }

    if (!def)
        throw "Unexpected data - definition is required [" + line + "]";

    en = def->clone();
    std::vector<double> v;
    std::istringstream is(line);
    for (double x; is >> x;) v.push_back(x);

    auto sz = en->items.size();
    if (v.size() != en->items.size())
        throw "Wrong number of values, expecting " + tos(sz)
        + ", got " + tos(v.size()) + " [" + line + "]";

    for (size_t i = 0; i < sz; i++)
        en->items[i]->x = en->items[i]->convert(true, v[i]);

    en->figureOutType();

    //ANG - noHint noKind
    //MAX - noKind Hint-if-noAngle
    //RNG - Kind Hint-if-noAngle

    if (en->type == RNG && !en->find<item::Kind>())
        en->items.push_back(Item::create(item::Kind::name()));

    if ( (en->type == RNG || en->type == MAX)
            && !en->find<item::Hint>()
            && !en->find<item::Angle>() )
        en->items.push_back(Item::create(item::Hint::name()));

    return en;
}

Item * Item::create(string name)
{
    using namespace item;
    if (0);
    else if (name == Velocity::name()) return new Velocity();
    else if (name == Max::name()) return new Max();
    else if (name == Range::name()) return new Range();
    else if (name == Fall::name()) return new Fall();
    else if (name == TopH::name()) return new TopH();
    else if (name == TopR::name()) return new TopR();
    else if (name == Time::name()) return new Time();
    else if (name == Vfin::name()) return new Vfin();

    else if (name == Kind::name()) return new Kind();
    else if (name == Hint::name()) return new Hint();

    else if (name == Angle::name_deg()) return new Angle(Angle::DEG);
    else if (name == Angle::name_mil()) return new Angle(Angle::MIL);
    else if (name == Angle::name_rad()) return new Angle(Angle::RAD);

    std::cout << "WARNING: dataset '" << name
              << "' is unknown and will be ignored\n";

    return new Unknown(name);
}

Entry * Entry::clone() const
{
    Entry * en = new Entry();
    for (auto p : items)
        en->items.push_back(p->clone());

    en->type = type;
    return en;
}

void Entry::figureOutType()
{
    // if Max -> MAX : no R
    // else if R -> RANGE : no Max
    // else -> ANGLE : Angle, no R, no Max

    item::Velocity v;
    item::Max m;
    item::Range r;

    if (find(&m)) type = MAX;
    else if (find(&r)) type = RNG;
    else type = ANG;

    // validate
    if (type == MAX)
    {
        if (find(&r)) throw "Cannot specify both Max and range";
        if (!find(&v)) throw "Cannot specify Max without V";
    }
}

bool Item::isUnknown() const
{
    if ( dynamic_cast<const item::Unknown *>(this) )
        return true;
    return false;
}


Item * Entry::find(Item * p) const
{
    if (p->isUnknown()) return nullptr;

    for ( auto q : items)
        if (typeid(*p) == typeid(*q))
            return q;

    return nullptr;
}

Dataset * Dataset::clone() const
{
    Dataset * ds = new Dataset(0);
    for (auto p : entries)
        ds->entries.push_back(p->clone());
    return ds;
}

void Dataset::run_inplace(Psi * psi)
{
    for (auto entry : entries)
        entry->run(psi);
}

void Entry::run(Psi * psi)
{
    Psi ps(*psi);
    if (type == DEF) never("Entry::run (1)");

    if (type == ANG) return run_ang(&ps);
    if (type == RNG) return run_rng(&ps);
    if (type == MAX) return run_max(&ps);

    throw "Internal error in Entry::run (3)";
}

void Entry::run_max(Psi * psi)
{
    Item * v = find<item::Velocity>();
    if (!v) never("Entry::run (2)");
    psi->p.velocity = v->x;

    TrResult tres;

    double hint = getHint();
    if (hint != INF_VAL )
    {
        Rsolver rs(psi, INF_VAL, TrType(TrType::MAX), hint);
        tres = rs.getFlat().first;
    }
    else
    {
        Rsolver rs(psi, INF_VAL, TrType(TrType::MAX));
        tres = rs.getFlat().first;
    }

    hint = r2d(tres.alpha);
    {
        Item * p = find<item::Kind>();
        if (p) p->x = TrType(TrType::MAX).dbl();
    }

    TrSolution res(tres, TrType(TrType::MAX), hint);

    for (auto p : items)
        p->set(res);
}

void Entry::run_ang(Psi * psi)
{
    Item * p = find<item::Velocity>();
    if (p) psi->p.velocity = p->x;
    p = find<item::Angle>();

    double angle = p->x;
    if (!p) never("Entry::run (4)");
    psi->s.angle_deg = angle;

    TrResult tres = Trajectory::run(psi);
    TrSolution res(tres, TrType(), angle);

    for (auto q : items)
        q->set(res);
}

void Entry::run_rng(Psi * psi)
{
    Item * iv = find<item::Velocity>();
    if (iv) psi->p.velocity = iv->x;
    Item * ir = find<item::Range>();
    if (!ir) never("Entry::run (5)");
    double rng = ir->x;

    Item * ikind = find<item::Kind>();
    if (!ikind) never("No kind");

    TrType kind(ikind->x);

    if (kind.type == TrType::MAX) never("type max");

    if (kind.type == TrType::UNKNOWN )
    {
        std::cout << "Guessing trajectory type " << std::flush;
        Rsolver rs(psi, rng, TrType());
        Rsolver::RsRes pres1 = rs.getFlat();
        TrResult tres1 = pres1.first;
        TrSolution res1(tres1, TrType(TrType::FLAT), r2d(tres1.alpha));

        if (pres1.second.type == TrType::MAX)
        {
            std::cout << "-> OUT OF REACH (using FLAT) " << r2d(tres1.alpha) << "\n";
            for (auto p : items) p->set(res1);
        }
        else if (pres1.second.type == TrType::FLAT)
        {
            Rsolver::RsRes pres2 = rs.getHigh();
            TrResult tres2 = pres2.first;
            TrSolution res2(tres2, TrType(TrType::HIGH), r2d(tres2.alpha));
            Entry * lo = clone(); for (auto p : lo->items) p->set(res1);
            Entry * hi = clone(); for (auto p : hi->items) p->set(res2);
            double ulo = util(lo);
            double uhi = util(hi);
            if (uhi < ulo)
            {
                std::cout << "-> HIGH ang=" << r2d(tres2.alpha);
                for (auto p : items) p->set(res2);
            }
            else
            {
                std::cout << "-> FLAT ang=" << r2d(tres1.alpha);
                for (auto p : items) p->set(res1);
            }
            std::cout << " u=" << ulo << ":" << uhi  << "\n";
            delete lo;
            delete hi;
        }
        else
            never("Entry::run_rng: type from Rsolver");

        // we done
        return;
    } // no kind

    // here we know kind
    double hint = getHint();

    Rsolver rs(psi);

    if (hint == item::Hint().x) rs.solve(rng, kind);
    else rs.solve(rng, kind, hint);

    Rsolver::RsRes pres = (kind.type == TrType::FLAT ? rs.getFlat() : rs.getHigh());
    TrResult tres = pres.first;
    TrSolution res(tres, kind, r2d(tres.alpha));
    for (auto p : items) p->set(res);
    return;
}


string Dataset::print() const
{
    if (entries.empty()) return "";
    ///std::ofstream of(dataset_out);
    std::ostringstream of;
    Entry * curdef = nullptr;
    for (auto p : entries)
    {
        if (!p->sameDef(curdef))
            of << (p->str(true)) << '\n';

        of << (p->str(false)) << '\n';
        curdef = p;
    }

    return of.str();
}

void Dataset::save() const
{
    if (entries.empty()) return;
    std::ofstream of(dataset_out);
    of << print();
    /*///
        Entry * curdef = nullptr;
        for (auto p : entries)
        {
            if (!p->sameDef(curdef))
                of << (p->str(true)) << '\n';

            of << (p->str(false)) << '\n';
            curdef = p;
        }
    */
}

bool Entry::sameDef(const Entry * q) const
{
    if (!q) return false;

    int sz = items.size();
    if (sz != int(q->items.size())) return false;

    for (int i = 0; i < sz; i++)
        if (typeid(*items[i]) != typeid(*q->items[i]))
            return false;

    return true;
}

string Entry::str(bool head) const
{
    string r, ln;

    if (head)
    {
        r += "\n";
        for (auto p : items)
            r += format(p->nm());

        while (ln.size() < r.size()) ln += '-';
        r += "\n" + ln;
    }
    else // values
    {
        for (auto p : items)
            r += format(p->convert(false, p->x));
    }
    return r;
}

double Dataset::util(const Dataset * d, std::vector<double> * v) const
{
    int sz = entries.size();
    if (sz != int(d->entries.size())) never("dataset");

    double sum = 0;
    for (int i = 0; i < sz; i++)
    {
        Entry * e1 = entries[i];
        Entry * e2 = d->entries[i];

        double ss = e1->util(e2);
        sum += ss;
        if (v) v->push_back(ss);
    }

    return sum;
}

double Entry::util(const Entry * d) const
{
    if ( !sameDef(d) ) never("incompatible entries");

    double sum = 0;
    for (size_t i = 0; i < items.size(); i++)
    {
        Item * e1 = items[i];
        Item * e2 = d->items[i];

        if ( e1->isUnknown() ) continue;

        sum += e1->util(e2);
    }

    return sum;
}

double Item::util(const Item * d) const
{
    if (x <= 0 || d->x <= 0)
    {
        std::cout << "WARNING: value in util function is zero or negative " << nm() << "=" << x << '\n';
        return 0;
    }

    double r = std::log(x) - std::log(d->x);
    r *= r;
    r *= 1000; // mil
    return r;
}

string Entry::getTypeStr() const
{
    if (0);
    else if ( type == ANG ) return "ANG";
    else if ( type == RNG ) return "RNG";
    else if ( type == MAX ) return "MAX";
    else never("type");
}

string Entry::getListStr(int sz) const
{
    string r;

    for ( auto q : items)
    {
        if ( typeid(*q) == typeid(item::Kind) ) continue;
        if ( typeid(*q) == typeid(item::Hint) ) continue;

        if ( !r.empty() ) r += ", ";
        r += q->nm() + "=" + tos(q->convert(false, q->x));
        if ( int(r.size()) > sz )
        {
            r += ", ...";
            break;
        }
    }

    return r;
}

string Entry::getKindStr() const
{
    Item * p = find<item::Kind>();
    if (p) return TrType(p->x).str();
    if ( type == MAX ) return TrType(TrType::MAX).str();
    return "N/A";
}

double Entry::getHint() const
{
    Item * p = find<item::Angle>();
    if (p) return p->x;

    p = find<item::Hint>();
    if (p) return p->x;

    return INF_VAL;
}

double Entry::getRange() const
{
    Item * p = find<item::Range>();
    if (p) return p->x;

    p = find<item::Max>();
    if (p) return p->x;

    return INF_VAL;
}

void Entry::setHint(double z)
{
    Item * p = find<item::Angle>();
    if (p) { p->x = z; return; }

    p = find<item::Hint>();
    if (p) { p->x = z; return; }

    never("Entry::setHint");
}

string Dataset::dump(const Dataset * ref) const
{
    std::vector<double> v;
    double u = util(ref, &v);
    auto refe = ref->getEntries();
    auto thse = getEntries();

    std::ostringstream os;

    for ( size_t i = 0; i < refe.size(); i++  )
    {
        Entry & ne = *thse[i];
        Entry & oe = *refe[i];

        double dh = ne.getHint();
        string sh = "N/A";
        if ( dh != INF_VAL ) sh = tos(dh);

        double rng = ne.getRange();
        string sr = "N/A";
        if ( rng != INF_VAL ) sr = tos(rng);

        os << ne.getTypeStr() << " U=" << v[i]
           << " type=" << ne.getKindStr()
           << " R=" << sr
           << " angle=" << sh << " (" << oe.getListStr(12) << ")\n";
    }

    os << std::setprecision(20);
    os << "U = " << u;

    return os.str();
}

string item::Angle::nm() const
{
    switch (units)
    {
        case DEG: return name_deg();
        case MIL: return name_mil();
        case RAD: return name_rad();
    }

    never(0);
}

double item::Angle::convert(bool load, double z)
{
    if ( units == MIL && load ) return r2d(mil2rad(z));
    if ( units == MIL && !load ) return rad2mil(deg2rad(z));
    if ( units == RAD && load ) return r2d(z);
    if ( units == RAD && !load ) return deg2rad(z);
    return x;
}


void Dataset::run_init(Psi * psi)
{
    Dataset * ds = clone();

    run_inplace(psi);

    // restore key values

    int sz = entries.size();
    if ( sz != (int) ds->entries.size() ) never("run_init");

    for (int i = 0; i < sz; i++ )
    {
        entries[i]->restore(ds->entries[i]);
    }
}

void Entry::restore(const Entry * e)
{
	if( type != e->type ) never("types");

	if( type == ANG ) return;

	if( type == RNG )
	{
	    Item * rt = find<item::Range>();
	    Item * re = e->find<item::Range>();
		if( !rt || !re ) never("range");

		rt->x = re->x;
		return;
	}

	if( type != MAX ) never("types");

	Item * mt = find<item::Max>();
	Item * me = e->find<item::Max>();
	if( !mt || !me ) never("range");

	mt->x = me->x;
	return;

	using std::cout;
	cout<<str(1)<<" AAA 1\n";
	cout<<str(0)<<getTypeStr()<<" AAA 2\n";
	cout<<e->str(0)<<e->getTypeStr()<<" AAA 3\n";

	never("Entry::restore NI: "+getTypeStr());
}


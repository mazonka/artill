

void Timer::init()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    tv_sec = t.tv_sec;
    tv_usec = t.tv_usec;
    adj = 0;
}

intint Timer::get()
{
    struct timeval this_call;
    gettimeofday(&this_call, NULL);
    intint r =
        (this_call.tv_sec - tv_sec) * 1000L
        + (this_call.tv_usec - tv_usec) / 1000;
    return r + adj;
}

void Timer::set(intint x)
{
    adj = 0;
    adj = x - get();
}

static string atleast2(int s)
{
    string m = tos(s);
    while ( m.size() < 2 ) m = string("0") + m;
    return m;
}

static char current_gmd[10];
static char current_hms[10];

string Timer::getGmd()
{
    if ( current_gmd[0] )
        return current_gmd;

    time_t t = ::time(0);
    struct tm * x = ::localtime(&t);
    string s;
    s += atleast2(x->tm_year + 1900);
    s += atleast2(x->tm_mon + 1);
    s += atleast2(x->tm_mday);
    return s;
}

void Timer::setGmd(string gmd)
{
    if ( !gmd.empty() && gmd.size() != 8 )
        return;
    std::strncpy(current_gmd, gmd.c_str(), 10);
}

void Timer::setHms(string hms)
{
    if ( !hms.empty() && hms.size() != 6 )
        return;
    std::strncpy(current_hms, hms.c_str(), 10);
}

string Timer::getHms()
{
    if ( current_hms[0] )
        return current_hms;

    time_t t = ::time(0);
    struct tm * x = ::localtime(&t);
    string s;
    s += atleast2(x->tm_hour);
    s += atleast2(x->tm_min);
    s += atleast2(x->tm_sec);
    return s;
}

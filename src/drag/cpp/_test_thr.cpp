// asynch example
#include <iostream>       // std::cout
#include <future>         // std::async, std::future
#include <chrono>         // std::chrono::milliseconds
#include <vector>

// a non-optimized way of checking for prime numbers:
bool is_prime (long long x, int y)
{
    bool m = true;
    for (long long i = 2; i < x; ++i) if (x % i == 0) m = false;
    return m;
}

int main ()
try
{
    // call function asynchronously:
    long long cn = 444444439;

    std::vector< std::future<bool> > fut;
    for ( int k = 0; k < 63; k += 2 )
    {
        fut.push_back( std::async (is_prime, cn + 2 + k, 1) );
    }

    // do something while waiting for function to set future:
    std::cout << "checking, please wait";
    std::chrono::milliseconds span (100);
    while (fut[0].wait_for(span) == std::future_status::timeout)
        std::cout << '.' << std::flush;

    std::vector<int> x;
    for ( int k = 0; k < (int)fut.size(); k++ )
    {
        x.push_back( fut[k].get() );
        std::cout << k << ' ' << std::flush;
    }

    std::cout << '\n';
    for ( auto i : x )
        if (i)
            std::cout << (cn += 2) << " "
                      << (i ? "is" : "is not") << " prime.\n";

    return 0;
}
catch (...)
{
    std::cout << "Exception thrown";
    return 1;
}

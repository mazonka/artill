class Binomial
{
public:
    Binomial(int Max)
    {
        max = Max+1;
        table = new unsigned int * [max]();
        for (int i=0; i < max; i++)
        {
            table[i] = new unsigned int[max]();

            for (int j = 0; j < max; j++)
            {
                table[i][j] = 0;
            }
        }
    }

    ~Binomial()
    {
        for (int i =0; i < max; i++)
        {
            delete table[i];
        }
        delete table;
    }

    unsigned int Choose(unsigned int n, unsigned int k);

private:
    bool Contains(unsigned int n, unsigned int k);

    int max;
    unsigned int **table;
};

unsigned int Binomial::Choose(unsigned int n, unsigned int k)
{
    if (n < k) return 0;
    if (k == 0 || n==1 ) return 1;
    if (n==2 && k==1) return 2;
    if (n==2 && k==2) return 1;
    if (n==k) return 1;


    if (Contains(n,k))
    {
        return table[n][k];
    }
    table[n][k] = Choose(n-1,k) + Choose(n-1,k-1);
    return table[n][k];
}

bool Binomial::Contains(unsigned int n, unsigned int k)
{
    if (table[n][k] == 0) 
    {
        return false;
    }
    return true;
}

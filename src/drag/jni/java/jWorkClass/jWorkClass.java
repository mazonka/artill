package jWorkClass;


public class jWorkClass
{
    static
    {
        ///Plat plat = new Plat();
        System.out.println("Loading shared objects: workclass jWorkClass");
        System.loadLibrary("jWorkClass");
        System.loadLibrary("wc_thin_imp");
    }

    private native long Create();
    private native int Delete(long g);
    private native int Calculate(long g, int x);
    private native double GetElement(long g, int x);

    // Inteface #1
    private native int SendData(long g, int nx, int ny, int [] x, double [] y);

    long mGs;

    public jWorkClass() { mGs = Create();}

    public void destroy()
    {
        if ( mGs < 0 ) return;
        Delete(mGs);
        mGs = -1;
    }

    protected void finalize() throws Throwable
    {
        destroy();
        super.finalize();
    }

    public int calculate(int x) { return Calculate(mGs, x); }
    public double getElement(int x) { return GetElement(mGs, x); }

    public int sendData(int nx, int ny, int [] x, double [] y)
    {
        return SendData(mGs, nx, ny, x, y);
    }
}



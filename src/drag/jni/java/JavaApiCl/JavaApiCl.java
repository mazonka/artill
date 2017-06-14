package JavaApiCl;


public class JavaApiCl
{
    static
    {
	///Plat plat = new Plat();
	System.out.println("Loading shared objects: apiclass JavaApiCl");
	System.loadLibrary("JavaApiCl");
	System.loadLibrary("apiclass");
    }

	public static final int OK = 0;
	public static final int IGNORED = 1;
	public static final int BADSIZE = 2;
	public static final int SAVEERROR =3;
	public static final int LOADERROR = 4;
	public static final int EXPORTERROR = 5;
	public static final int IMPORTERROR = 6;
	public static final int BADINDEX = 7;
	
	private native long Create();
	private native int Delete(long g);
	private native int Calculate(long g, int x);
	private native double GetElement(long g, int x);

	// Inteface #1
	private native int SendData(long g, int nx, int ny, int [] x, double [] y);

	long mGs;

	public JavaApiCl(){ mGs = Create();}

	public void destroy()
	{
		if( mGs < 0 ) return;
		Delete(mGs); 
		mGs = -1; 
	}

	protected void finalize() throws Throwable
	{
		 destroy();
		 super.finalize();
	}

	public int calculate(int x){ return Calculate(mGs,x); }
	public double getElement(int x){ return GetElement(mGs,x); }
	
	public int sendData(int nx, int ny, int [] x, double [] y)
	{
		return SendData(mGs,nx,ny,x,y);
	}
}



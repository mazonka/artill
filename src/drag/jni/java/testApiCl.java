import JavaApiCl.JavaApiCl;

public class testApiCl {

	public static void main(String[] args) 
	{
		JavaApiCl gs = new JavaApiCl();


		int [] x = { 50,50, 1, 1, 1 };
		double [] y = { -0.6, 1, 1,1,1,1, 0,0,0,0 };

		gs.sendData(x.length,y.length,x,y);
		int rk = gs.calculate(123);

		for( int i=0; i<4; i++ )
		{
			if(i!=0) System.out.print(", ");
			System.out.print( ""+gs.getElement(i) );
		}
		System.out.println("");


		gs.destroy();

		System.out.println("Result1="+rk);

		gs.destroy();
	}
}

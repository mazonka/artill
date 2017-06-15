import jWorkClass.*;

public class testJWC
{

    public static void main(String[] args)
    {
        DragMod x = new DragMod();

	x.run("test");

	return;

/*
        jWorkClass gs = new jWorkClass();


        int [] x = { 50, 50, 1, 1, 1 };
        double [] y = { -0.6, 1, 1, 1, 1, 1, 0, 0, 0, 0 };

        gs.sendData(x.length, y.length, x, y);
        int rk = gs.calculate(123);

        for ( int i = 0; i < 4; i++ )
        {
            if (i != 0) System.out.print(", ");
            System.out.print( "" + gs.getElement(i) );
        }
        System.out.println("");


        gs.destroy();

        System.out.println("Result1=" + rk);

        gs.destroy();
*/
    }
}

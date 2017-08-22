import jWorkClass.*;

public class testJWC
{

    static void testDragMod()
    {
        DragMod x = new DragMod();

        // test1
        //x.run("test");

        // test 2
        //x.setio("hi");
        //x.set_input("hello");
        //System.out.println("\nout: " + x.get_output());

        // test 3

        set_proj(x);
        set_shot(x);
        set_inte(x);
        set_cons(x);
        set_cdgr(x);

        x.run("shoot");
        x.setio("out/trres.dat");
        System.out.println("\nout:\n" + x.get_output());


        return;
    }

    static void testDmExample()
    {
	DmExample.Proj proj = new DmExample.Proj();
	DmExample.Drag drag = new DmExample.Drag();
	
        DmExample x = new DmExample(proj,drag);
	DmExample.Traj traj = x.range(2000,true);
	drag = x.solve();
    }

    public static void main(String[] args)
    {
        testDragMod();
	testDmExample();
    }

    static void set_proj(DragMod x)
    {
        String proj =
            "mass 0.0162\n"
            + "diameter 0.0086\n"
            + "length 0.02\n"
            + "velocity 915\n"
            + "cdtype 1\n";

        x.setio("in/proj.dat");
        x.set_input(proj);
    }

    static void set_shot(DragMod x)
    {
        String shot =
            "angle_deg 10\n"
            + "angle_dlt 0\n"
            + "height 0\n"
            + "ntraj 1\n"
            + "turbulence 0\n"
            + "zwind 0\n"
            + "xwind 0\n";

        x.setio("in/shot.dat");
        x.set_input(shot);
    }

    static void set_inte(DragMod x)
    {
        String inte =
            "max_len 1e8\n"
            + "dt0 0.1\n"
            + "dtmax 0.005\n"
            + "precision 1e-15\n"
            + "dEpm_rej 3\n"
            + "dEpm_max 2\n"
            + "dEpm_min 1\n"
            + "dh_up 1.5\n"
            + "dh_dn 0.5\n"
            + "renormE 10\n"
            + "dragcare 0.5\n";

        x.setio("in/integr.dat");
        x.set_input(inte);
    }


    static void set_cons(DragMod x)
    {
        String cons =
            "Earth_radius 6378137\n"
            + "Earth_mass 5.9722e24\n"
            + "Ggrav 6.67408e-11\n"
            + "std_press 101325\n"
            + "temp_rate 6.5\n"
            + "Earth_atm_R 6356.766\n"
            + "std_temp 288.15\n"
            + "grav_atm 9.80665\n"
            + "molec_w 28.9644\n"
            + "gas_const 8.31432\n"
            + "speed_1k 20.046\n";

        x.setio("in/consts.dat");
        x.set_input(cons);
    }

    static void set_cdgr(DragMod x)
    {
        String cdgr =
            "#range 0.3 3\n"
            + "0 0.2\n"
            + "0 0.2\n";

        x.setio("in/cdgraph.dat");
        x.set_input(cdgr);
    }

}

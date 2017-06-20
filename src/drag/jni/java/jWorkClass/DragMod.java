package jWorkClass;


public class DragMod
{
    jWorkClass thin = new jWorkClass();

    public int run(String cmd)
    {
        return senddata(new Protocol('R', cmd));
    }

    public int setio(String name)
    {
        return senddata(new Protocol('S', name));
    }

    public int set_input(String data)
    {
        return senddata(new Protocol('I', data));
    }

    public String get_output()
    {
        String r = "";

        for ( int i = 0; ; i++ )
        {
            int x = thin.calculate(i);
            if ( x == 0 ) break;
            r += (char)x;
        }

        return r;
    }

    int senddata(Protocol  p)
    {
        int [] x = p.mk_arr();
        int k = thin.sendData(x.length, 0, x, new double[0]);
        return k;
    }

    static int [] ut_s2a(String s)
    {
        int [] r = new int [s.length()];

        for ( int i = 0; i < s.length(); i++ )
            r[i] = (int)(s.charAt(i));

        return r;
    }

    class Protocol
    {
        char func;
        String text;
        public Protocol(char c, String s)
        {
            func = c;
            text = s;
        }

        int [] mk_arr()
        {
            return ut_s2a(func + text);
        }
    }
}



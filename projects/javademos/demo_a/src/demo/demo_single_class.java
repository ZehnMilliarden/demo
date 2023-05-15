package demo;

public class demo_single_class {

    private String m_strValues;
    private static final demo_single_class _instanc = new demo_single_class();

    public static final demo_single_class Instance()
    {
        return _instanc;
    }

    private demo_single_class() {}

    public void set_default_values(String  defaultValue)   {
        m_strValues = defaultValue;
    }  

    public String get_default_values()
    {
        return m_strValues; //
    }
}

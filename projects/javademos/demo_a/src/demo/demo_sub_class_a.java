package demo;

public class demo_sub_class_a extends demo_class_a {
    public demo_sub_class_a()
    {
        super.m_nDefaultProtectValues = 1000;
    }

    public void set_default_values(int nDefaultValues) {   super.m_nDefaultProtectValues = nDefaultValues + 10; }  //
    public int get_default_values() { return super.m_nDefaultProtectValues; } //
}

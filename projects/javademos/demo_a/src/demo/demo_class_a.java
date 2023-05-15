package demo;

public class demo_class_a {

    int m_nDefaultProtectValues;
    private int m_nPrivateProtectValues;
    public int m_nPublicProtectValues;
    protected int m_nProtectedProtectValues;

    public demo_class_a()
    {
        m_nDefaultProtectValues = 0;
        m_nPrivateProtectValues = 0;
        m_nPublicProtectValues = 0;
        m_nProtectedProtectValues = 0;
    }

    public void set_private_values(int nValues)  {   m_nPrivateProtectValues   =   nValues; }  //  
    public int  get_private_values() { return m_nPrivateProtectValues; } // 
    public void set_public_values(int nValues) { m_nPublicProtectValues = nValues;} // 
    public int get_public_values() { return m_nPublicProtectValues;}  // 
    public void  set_default_values(int nDefaultValues) { m_nDefaultProtectValues = nDefaultValues;} // 
    public int get_default_values() { return m_nDefaultProtectValues;} // 
    public void set_protected_values(int nProtectedValues) { m_nProtectedProtectValues  = nProtectedValues;} //
    public int get_protected_values() { return m_nProtectedProtectValues;} //  
}

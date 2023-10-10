
#[derive(Debug)]
pub struct subtest2 {
    pub name: String,
    pub id: String,
    pub name_sp: String,
    pub id_sp: String, 
}

impl subtest2
{
    pub fn make_sub_test2_specail_info(&mut self, name: &String, id: &String)
    {
        self.name_sp = name.clone();
        self.id_sp = id.clone();
    }

    pub fn get_sub_test2_specail_name(&self) -> &String
    {
        return &self.name_sp;
    }

    pub fn get_sub_test2_specail_id(&self) -> &String
    {
        return &self.id_sp;
    }
}

impl crate::include::iterface::InterfaceCommon for subtest2
{
    fn get_name(&self) -> &String
    {
        return &self.name;
    }

    fn get_id(&self) -> &String
    {
        return &self.id;
    }

    fn set_name(&mut self, name: &String)
    {
        self.name = name.clone();
    }

    fn set_id(&mut self, id: &String)
    {
        self.id = id.clone();
    }
}

use crate::sub1::subtest1 as sb1;

pub fn sub_test2_test_module()
{
    let sb1obj : sb1::Subtest1 = sb1::Subtest1::create();
    println!("{:?} in subtest2.rs", sb1obj);
}
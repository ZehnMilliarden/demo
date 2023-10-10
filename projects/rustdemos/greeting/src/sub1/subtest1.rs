
#[derive(Debug)]
pub struct Subtest1 {
    name: String,
    id: String,
    name_sp: String,
    id_sp: String, 
}

impl Subtest1
{
    pub fn make_sub_test1_specail_info(&mut self, name: &String, id: &String)
    {
        self.name_sp = name.clone();
        self.id_sp = id.clone();
    }

    pub fn get_sub_test1_specail_name(&self) -> &String
    {
        return &self.name_sp;
    }

    pub fn get_sub_test1_specail_id(&self) -> &String
    {
        return &self.id_sp;
    }

    pub fn create() -> Subtest1
    {
        return Subtest1{
            name: String::from("SubTest1 Name"),
            id: String::from("SubTest1 Id"),
            name_sp: String::from("SubTest1 Sp Name"),
            id_sp: String::from("SubTest1 Sp Id"),
        };
    }
}
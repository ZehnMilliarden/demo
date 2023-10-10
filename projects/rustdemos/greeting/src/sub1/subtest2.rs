#[derive(Debug, Clone)]
pub struct Subtest2 {
    name: String,
    id: u32,
}

impl Subtest2 
{
    pub fn create(name: String, id: u32) -> Subtest2
    {
        return Subtest2{
            name: name,
            id: id,
        };
    }

    pub fn get_name(&self) -> &String
    {
        return &self.name;
    }

    pub fn get_id(&self) -> &u32
    {
        return &self.id;
    }

    pub fn get_name_sp(&self) -> String
    {
        return self.name.clone();
    }
}
pub trait InterfaceCommon
{
    fn get_name(&self) -> &String;
    fn get_id(&self) -> &String;
    fn set_name(&mut self, name: &String);
    fn set_id(&mut self, id: &String);
}
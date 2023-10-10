use crate::sub1::subtest2::Subtest2 as Subtest2;

pub fn entry()
{
    let t1 : Subtest2 = Subtest2::create(String::from("test name"), 123);
    let t2 : Subtest2 = t1.clone();
    let t3: String = t2.get_name().clone();
}
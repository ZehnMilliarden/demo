use crate::sub1::subtest1 as sb1;

pub fn entry()
{
    let sb1obj: sb1::Subtest1 = sb1::Subtest1::create();
    let tem = sb1obj.get_sub_test1_specail_name();
    println!("{} is temp value", tem)
}
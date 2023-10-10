use crate::sub2::subtest2 as sb2;

pub fn entry()
{
    let sb2obj: sb2::subtest2 = sb2::subtest2{
        name: String::from("test name"),
        id: String::from("test id"),
        name_sp: String::from("test name sp"),
        id_sp: String::from("test id sp")
    };

    let tem = sb2obj.get_sub_test2_specail_id();
    println!("{} is temp value", tem);

    sb2::sub_test2_test_module();
}
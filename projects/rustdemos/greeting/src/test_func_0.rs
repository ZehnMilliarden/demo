
use crate::test_func_def::{
    self as test_func_def, 
    *,
};

pub fn entry()
{
    let domain: String = String::from("domain");
    let name: String = String::from("name");
    let nation : String = String::from("nation");
    let mut s1 = test_func_def::Site::create(domain, name, nation, 0);

    println!("{:?}", s1);
    s1.println(String::from("12345").to_owned());
    println!("{:?}", s1);

    let books: test_func_def::Book = test_func_def::Book::Papery(123);
    let ret: String = match books {
        test_func_def::Book::Papery(_) => {
            String::from("value1")
        },
        test_func_def::Book::Electronic(_) => {
            String::from("value2")
        },
        _ => {
            String::from("value3")
        }
    };

    println!("{:?}", ret);
    println!("{:?}", books);

    let opt_pre =String::from("value_pre_zzz").to_owned();
    let opt: Option<&String> = Option::Some(&opt_pre);
    match opt {
        Option::Some(some_value) =>{
            println!("some_value: {}", some_value);
        },
        Option::None => println!("None")
    }

    let mut vv : test_func_def::StructName = test_func_def::StructName::create();
    println!("{:?}", vv);
    vv.modify_member(&String::from("target"));
    vv.modify_abstract_member1(10);
    println!("{}, {}", vv.get_member(), vv.get_abstract_member1());
    test_func_def::call_back(&mut vv);
    println!("{}, {}", vv.get_member(), vv.get_abstract_member1());
    test_func_def::call_back_2(&mut vv);
    println!("{}, {}, {}", vv.get_member(), vv.get_abstract_member1(), vv.get_abstract_member2());
    test_func_def::call_back_3(&mut vv);
    println!("{}, {}, {}", vv.get_member(), vv.get_abstract_member1(), vv.get_abstract_member2());

    struct Str<'a> {
        content: &'a str
    }
    let s: Str<'_> = Str {
        content: "string_slice"
    };
    println!("s.content = {}", s.content);

    let r;
    {
        let s1 = "rust";
        let s2 = "ecmascript";
        r = test_func_def::longer(s1, s2);
    }
    println!("{} is longer", r);
}
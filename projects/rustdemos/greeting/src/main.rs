#[derive(Debug)]
struct Site {
    domain: String,
    name: String,
    nation: String,
    found: u32
}

impl Site {
    fn println(&mut self, pre: String) -> u32
    {
        println!("{},{:?}", pre, self);
        self.name.push_str("test");
        return 123;
    }
}

#[derive(Debug)]
enum Book {
    Papery(u32),
    Electronic(String)
}

#[derive(Debug)]
struct StructName
{
    member0: String,
    member1: i32,
    member2: u8,
}

impl StructName {
    fn get_member(&self) -> &String{
        return &self.member0;
    }

    fn modify_member(&mut self, target: &String) {
        self.member0 = target.clone();
    }

    fn create() -> StructName
    {
        let v: StructName = StructName{
            member0: String::from("Member0"),
            member1: 1,
            member2: 2
        };
        return v;
    }
}

trait AbstractStructMember {
    fn get_abstract_member1(&self) -> i32;
    fn modify_abstract_member1(&mut self, target: i32);
}

impl AbstractStructMember for StructName {
    fn get_abstract_member1(&self) -> i32
    {
        return self.member1;
    }

    fn modify_abstract_member1(&mut self, target: i32)
    {
        self.member1 = target;
    }
}

trait AbstractStructMember2 {
    fn get_abstract_member2(&self) -> u8;
    fn modify_abstract_member2(&mut self, target: u8);
}

impl AbstractStructMember2 for StructName {
    fn get_abstract_member2(&self) -> u8
    {
        return self.member2;
    }

    fn modify_abstract_member2(&mut self, target: u8)
    {
        self.member2 = target;
    }
}

fn call_back(object: &mut impl AbstractStructMember)
{
    object.modify_abstract_member1(100);
}

fn call_back_2<T>(object: &mut T)
where 
    T: AbstractStructMember + AbstractStructMember2
{
    object.modify_abstract_member1(1000);
    object.modify_abstract_member2(200);
}

fn longer<'live>(s1: &'live str, s2: &'live str) -> &'live str {
    if s2.len() > s1.len() {
        s2
    } else {
        s1
    }
}

mod include;
mod sub2;
mod sub1;

use sub2::subtest2 as sb2;
use sub1::subtest1 as sb1;

fn test_func_1()
{
    let sb1obj: sb1::Subtest1 = sb1::Subtest1::create();
    let tem = sb1obj.get_sub_test1_specail_name();
    println!("{} is temp value", tem)
}

fn test_func_2()
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

fn main() {
    let domain: String = String::from("domain");
    let name: String = String::from("name");
    let mut s1 = Site {
        domain,
        name,
        nation: String::from("nation"),
        found: 123,
    };

    println!("{:?}", s1);
    s1.println(String::from("12345").to_owned());
    println!("{:?}", s1);

    let books: Book = Book::Papery(123);
    let ret: String = match books {
        Book::Papery(_) => {
            String::from("value1")
        },
        Book::Electronic(_) => {
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

    let mut vv : StructName = StructName::create();
    println!("{:?}", vv);
    vv.modify_member(&String::from("target"));
    vv.modify_abstract_member1(10);
    println!("{}, {}", vv.get_member(), vv.get_abstract_member1());
    call_back(&mut vv);
    println!("{}, {}", vv.get_member(), vv.get_abstract_member1());
    call_back_2(&mut vv);
    println!("{}, {}, {}", vv.get_member(), vv.get_abstract_member1(), vv.get_abstract_member2());

    struct Str<'a> {
        content: &'a str
    }
    let s = Str {
        content: "string_slice"
    };
    println!("s.content = {}", s.content);

    let r;
    {
        let s1 = "rust";
        let s2 = "ecmascript";
        r = longer(s1, s2);
    }
    println!("{} is longer", r);

    test_func_2();

}

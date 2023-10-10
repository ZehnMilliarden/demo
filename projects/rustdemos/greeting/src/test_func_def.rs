#[derive(Debug)]
    pub struct Site {
        domain: String,
        name: String,
        nation: String,
        found: u32
    }
    
    impl Site {
        pub fn println(&mut self, pre: String) -> u32
        {
            println!("{},{:?}", pre, self);
            self.name.push_str("test");
            return 123;
        }
    
        pub fn create(domain: String, name: String, nation: String,  found: u32) -> Site
        {
            let v: Site = Site{
                domain,
                name,
                nation,
                found
            };
            return v;
        }
    }
    
    #[derive(Debug)]
    pub enum Book {
        Papery(u32),
        Electronic(String)
    }
    
    #[derive(Debug)]
    pub struct StructName
    {
        member0: String,
        member1: i32,
        member2: u8,
    }
    
    impl StructName {
        pub fn get_member(&self) -> &String{
            return &self.member0;
        }
    
        pub fn modify_member(&mut self, target: &String) {
            self.member0 = target.clone();
        }
    
        pub fn create() -> StructName
        {
            let v: StructName = StructName{
                member0: String::from("Member0"),
                member1: 1,
                member2: 2
            };
            return v;
        }
    }
    
    pub trait AbstractStructMember {
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
    
    pub trait AbstractStructMember2 {
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

    pub trait AbstractStructMember3 {
        fn get_abstract_member3(&self) -> u8;
        fn modify_abstract_member3(&mut self, target: u8);
    }
    
    // obj 必须是需要了 AbstractStructMember 特性
    pub fn call_back(object: &mut impl AbstractStructMember)
    {
        object.modify_abstract_member1(100);
    }
    
    // obj 实现了 AbstractStructMember 和 AbstractStructMember2 特性
    pub fn call_back_2<T>(object: &mut T)
    where 
        T: AbstractStructMember + AbstractStructMember2
    {
        object.modify_abstract_member1(1000);
        object.modify_abstract_member2(200);
    }

    // obj 必须是实现了 AbstractStructMember2 特性
    pub fn call_back_3<T: AbstractStructMember2>(object: &mut T)
    {
        object.modify_abstract_member2(201);
    }
    
    // obj 必须是实现了 AbstractStructMember3 特性
    pub fn call_back_4<T: AbstractStructMember3>(object: &mut T)
    {
        object.modify_abstract_member3(202);
    }

    pub fn longer<'live>(s1: &'live str, s2: &'live str) -> &'live str {
        if s2.len() > s1.len() {
            s2
        } else {
            s1
        }
    }
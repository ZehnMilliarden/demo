import demo.demo_class_a;
import demo.demo_sub_class_a;
import demo.demo_single_class;

public class App {
    public static void main(String[] args) throws Exception {
        System.out.println("Hello, World!");
        demo_class_a demo = new demo_class_a();
        demo.set_default_values(100);
        System.out.println(demo.get_default_values());

        demo_sub_class_a demo_sub = new demo_sub_class_a();
        demo_class_a demo_abstract = demo_sub;

        demo_abstract.set_default_values(2000);
        System.out.println(demo_abstract.get_default_values());

        demo_single_class.Instance().set_default_values("this is a demo");
        System.out.println(demo_single_class.Instance().get_default_values());
    }
}

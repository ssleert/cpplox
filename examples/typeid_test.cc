#include <iostream>
#include <memory>
#include <exception>
#include <stdexcept>
#include <typeinfo>

#define fn auto

class Base {
public:
        virtual ~Base() {};
        template<class T>
        fn test(T a) -> T {
                std::cout << "from base\n";
                return T();
        }
};

class Child : public Base {
public:
        template<class T>
        fn test(T a) -> T {
                std::cout << "from child\n";
                return a;
        }
};

class SecondChild : public Base {
public:
        template<class T>
        fn test(T a) -> T {
                std::cout << "from SecondChild\n";
                return a + a;
        }
};

fn main() -> int {
        Child* cp = new Child;
        SecondChild* cp2 = new SecondChild;
        Base* bp = cp;

        if (typeid(*bp) == typeid(Child)) {
                Child* c;
                try {
                        c = dynamic_cast<Child*>(bp);
                } catch(...) {
                        throw;
                }
                std::cout << "yeaaa is equals\n";
                std::cout << c->test(123) << "\n";
                std::cout << c->test(123.3254) << "\n";
                std::cout << c->test(std::string("swag")) << "\n";
                bp = cp2;
                auto b = dynamic_cast<Base*>(bp);
                std::cout << b->test(123) << "\n";
                std::cout << b->test(123.3254) << "\n";
                std::cout << b->test(std::string("swag")) << "\n";
        }

        delete cp;
        delete cp2;
}

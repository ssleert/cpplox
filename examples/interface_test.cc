#include <iostream>

#define fn auto

class Base {
public:
        virtual fn swag() -> void = 0;
};

class Derived : public Base {
public:
        fn swag() -> void {
                std::cout << "swag\n";
        }
};


fn consumer(Base& b) -> void {
        b.swag();
}


fn main() -> int {
        Derived d;
        Base& b = d;
        consumer(b);
}

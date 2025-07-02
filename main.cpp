#include <iostream>
#include "macro_final.h"
#include <chrono>
#include <fstream>
#include <string>



class Car {
public:
    virtual void Engine() {
        // std::cout << "Cars have engines\n";
    }
    virtual ~Car() = default;
};

class Dodge FINAL : public Car {
public:
    void Engine() FINAL override {
        //std::cout << "Dodge has a v6 engine\n";
    }
};


void check_engine(Car* c) {
    c->Engine();  // Virtual call — can only be devirtualized if final
}


int main() {
   
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        Car* a = new Dodge(); // will not be inlined if LTO is disabled
        a->Engine();
        delete a;
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed1 = end1 - start1;
    std::cout << "Time for Car* calls: " << elapsed1.count() << " ms\n";

    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        Dodge* b = new Dodge(); // will be inlined regardless of final because it’s a direct call.
        b->Engine();
        delete b;
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed2 = end2 - start2;
    std::cout << "Time for Dodge* calls: " << elapsed2.count() << " ms\n";


    auto start3 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        Dodge* d = new Dodge(); // will be inlined only if final keyword is present of final because it’s a direct call.
        check_engine(d);
        delete d;
    }
    auto end3 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed3 = end3 - start3;
    std::cout << "Time for Dodge* calls: " << elapsed3.count() << " ms\n";

    return 0;
}

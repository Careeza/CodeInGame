#include <functional>
#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <array>


enum struct Action_type {
	seed,
	grow,
	complete,
	wait,
	none,
};

void printA() {
    int a = 42;
}

void printB() {
    int a = 42;
}

void printC() {
    int a = 42;
}

void printD() {
    int a = 42;
}

void    test1() {
    std::map<Action_type, std::function<void(void)>>    t;

    t[Action_type::seed] = printA;
    t[Action_type::grow] = printB;
    t[Action_type::complete] = printC;
    t[Action_type::wait] = printD;

    const std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
    long long unsigned time;
    for (int i = 0; i < 1000000; i++) {
        t[Action_type::seed]();
        t[Action_type::grow]();
        t[Action_type::complete]();
        t[Action_type::wait]();
    }
    const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    std::cerr << "TIME1 = " << time / 1000 << std::endl;
}

void    test2() {
    std::vector<std::function<void(void)>>  t;

    t.push_back(printA);
    t.push_back(printB);
    t.push_back(printC);
    t.push_back(printD);

    const std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
    long long unsigned time;

    for (int i = 0; i < 1000000; i++) {
        t[static_cast<int>(Action_type::seed)]();
        t[static_cast<int>(Action_type::grow)]();
        t[static_cast<int>(Action_type::complete)]();
        t[static_cast<int>(Action_type::wait)]();
    }
    const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    std::cerr << "TIME2 = " << time / 1000 << std::endl;
}

void    test3() {
    std::function<void(void)> t[4];

    t[0] = printA;
    t[1] = printB;
    t[2] = printC;
    t[3] = printD;

    const std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
    long long unsigned time;

    for (int i = 0; i < 1000000; i++) {
        t[static_cast<int>(Action_type::seed)]();
        t[static_cast<int>(Action_type::grow)]();
        t[static_cast<int>(Action_type::complete)]();
        t[static_cast<int>(Action_type::wait)]();
    }

    const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    std::cerr << "TIME3 = " << time / 1000 << std::endl;
}

void    test4() {
    std::array<std::function<void(void)>, 4> t;

    t[0] = printA;
    t[1] = printB;
    t[2] = printC;
    t[3] = printD;

    const std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
    long long unsigned time;
    
    for (int i = 0; i < 1000000; i++) {
        t[static_cast<int>(Action_type::seed)]();
        t[static_cast<int>(Action_type::grow)]();
        t[static_cast<int>(Action_type::complete)]();
        t[static_cast<int>(Action_type::wait)]();
    }

    const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    std::cerr << "TIME4 = " << time / 1000 << std::endl;
}


int     main(void) {
   test2();
   test3();
   test4();
   test1();
}
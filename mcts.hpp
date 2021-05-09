#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <iostream>

// class Action {
//     public:
//         virtual void execute(T& state) = 0;
// };


struct Tree {

    std::vector<Tree>   child;
    Tree                *father;

    double w;
    double n;
};

template <class T, class U, class V>
class MCTS {
    public:

    private:
        T state;
        U action;
        V strategy;
};


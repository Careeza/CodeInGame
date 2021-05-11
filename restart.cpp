#include "common.hpp"

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <random>
#include <array>
#include <sstream>
#include <algorithm>
#include <functional>
#include <fstream>

class Tree {
    private:
        std::vector<Tree>   child;
        std::vector<Action> actions;
        Tree                *father;
        State               state;
        double              w;
        double              n;

    public:
        explicit Tree(const State& s, Tree *f);
        Tree    *select_tree();
        void    expand();
        double  simulate();
        void    back_track(double score);
        bool    explore() const;
        int     nb_exploration() const;
        double  score() const;
		int		child_size() {return child.size();};
		int		get_player() {return state.info.player;};
        Action  best_action() const;

        void    print(std::string space = "") {

            std::cout << space << "STATE" << std::endl;
            std::cout << space << "n = " << n << std::endl;
            std::cout << space << "w = " << w << std::endl;

            space = space + "    ";

            for (int i = 0; i < child.size(); i++) {
                std::cout << space << "CHILD no " << i + 1 << std::endl;
                std::cout << space << "Actions = " << actions[i] << std::endl;
                double UCT_SCORE = (1 - child[i].score()) + sqrt(2) * sqrt(log2(n) / child[i].n);
                std::cout << space << "UCT SCORE = " << UCT_SCORE << std::endl;
                std::cout << space << "N = " << child[i].n << std::endl;
                std::cout << space << "W = " << child[i].w << std::endl;
                std::cout << space << "Score = " << 1 - child[i].score() << std::endl;
                //child[i].print(space);
                std::cout << std::endl;
            }
            //std::cout << std::endl
        }
};

Tree::Tree(const State& s, Tree *f) : state(s), father(f), w(1), n(1) {}

Tree    *Tree::select_tree() {
    double best_score = -10;
    Tree *best_tree = NULL;

    if (child.size() == 0)
        return this;
    
    int i = 0;
    for (Tree& t : child) {
        if (t.n == 0) {
            std::cerr << "PAS NORMAL DANS MON CAS" << std::endl;
            return this;
        }
        double score = n - t.n;
        if (score > best_score) {
            best_score = score;
            best_tree = &t;
        }
        i++;
    }
    return best_tree->select_tree();
}

void    Tree::expand() {
	state.generate_all_actions(actions);
    for (Action a : actions) {
		State new_state;
		state.simulate_action(new_state, a);
        child.emplace_back(new_state, this);
    }
}


double  Tree::simulate() {
    State simulation(state);

    while (1) {
        if (simulation.final_state()) {
            if (state.info.player) {
                return 1 - simulation.eval_state();
            } else {
                return simulation.eval_state();
            }
        }
        Action a = best_actionH(simulation);
		simulation.do_action(a);
    }
    return 0;
}

Action  Tree::best_action() const {
    double     best_score = 100000;
    int     index = 0;
    int     best_index = 0;
    
    std::cout << " ----------- " << std::endl;
	for (const Tree& t : child) {
        double score = 1 - t.score();
        std::cout << actions[index] << " : ";;
		std::cerr << "Score " << score << std::endl;
        if (score < best_score) {
            best_score = score;
            best_index = index;
        } else {
            std::cerr << "PAS PLUS PETIT LUL" << std::endl;
        }
        index++;
    }

    return actions[best_index];
}

void    Tree::back_track(double score) {
    n += 1;
    w += score;
    if (father != NULL)
        father->back_track(1 - score);
}

bool    Tree::explore() const {
    return (n != 0);
}

double  Tree::score() const {
    return w / n;
}

int     Tree::nb_exploration() const {
    return static_cast<int>(n);
}

Action  best_moove(State& s) {
    Tree root(s, NULL);
    root.expand();
    const std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
    long long unsigned time;
    int i = 0;
    do {
        Tree *leaf = root.select_tree();
        if (leaf->explore()) {
            leaf->expand();
            leaf = leaf->select_tree();
        }
        double score = leaf->simulate();
        leaf->back_track(score);
        const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        i++;
    } while (time < 100000);


    std::cerr << " BEST ACTION " << std::endl;
    Action a = best_actionH(s);
    Action b = root.best_action();

    if (a.type != b.type) {
        std::cerr << "H :" << a << std::endl;
        std::cerr << "B :" << b << std::endl;
        root.print();
        exit (0);
    }

    return root.best_action();
}
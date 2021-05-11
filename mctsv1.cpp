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

        Action  best_action() const;
};

Tree::Tree(const State& s, Tree *f) : state(s), father(f), w(1), n(1) {}

Tree    *Tree::select_tree() {
    double best_score = -10;
    Tree *best_tree = NULL;

    if (child.size() == 0)
        return this;
    
    int i = 0;
    for (Tree& t : child) {
		if (t.get_player() == state.info.player) {
		}
        if (t.n == 0) {
            return t.select_tree();
        }
        double score = n / t.n;
        if (score > best_score) {
            best_score = score;
            best_tree = &t;
        }
        i++;
    }
    return best_tree->select_tree();
}

void    Tree::expand() {
    //actions = state.actions_avaible();
	state.generate_all_actions(actions);
    for (Action a : actions) {
		State new_state;
		state.simulate_action(new_state, a);

		if (state.info.player == new_state.info.player) {
		}

        child.emplace_back(new_state, this);
    }
	//std::cerr << "here " << child.size() << std::endl;
}

double  Tree::simulate() {
    State simulation(state);

    while (1) {
        if (simulation.final_state()) {
            if (state.info.player)
                return 1 - simulation.eval_state();
            else
                return simulation.eval_state();
        }
        Action a = best_actionH(simulation);
		simulation.do_action(a);
    }
    return 0;
}

Action  Tree::best_action() const {
    int     best_score = 10000;
    int     index = 0;
    int     best_index = 0;
    
	for (const Tree& t : child) {
        int score = t.w;
		//std::cerr << "Score " << score << std::endl;
        if (score < best_score) {
            best_score = score;
            best_index = index;
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
    //return best_actionH(s);

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
    std::cerr << i << std::endl;

    Action a = best_actionH(s);
    Action b = root.best_action();

    std::cerr << "H :" << a << std::endl;
    std::cerr << "B :" << b << std::endl;

    if (a.type != b.type) {
        root.print();
        exit (0);
    }

    // if (s.info.days == 0) {
    //     root.print();
    //     exit (0);
    // }

    return root.best_action();
}
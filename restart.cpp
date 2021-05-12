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
        double  simulate(bool disp = false);
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

Tree::Tree(const State& s, Tree *f) : father(f), state(s), w(1), n(1) {}

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

static void    print_map(std::array<Cell, 37> map, Info info, std::vector<std::string> possible_moves) {
    std::cerr << "-----MAP-----" << std::endl;
	std::cerr << 37 << std::endl;
	for (int i = 0; i < 37; i++) {
		int richness = map[i].richness;
		std::cerr << i << " " << richness  << " " << 42 << " " << 42 << " " << 42 << " " << 42 << " " << 42 << " " << 42 << std::endl;
	}
    int day = info.days;
    int nutrients = info.nutriments;

	std::cerr << day << std::endl;
	std::cerr << nutrients << std::endl;

	int sun = info.sun[0];
	int score = info.score[0];
	std::cerr << sun << " " << score << std::endl;

	int wait = info.wait[1];
	sun = info.sun[1];
	score = info.score[1];
	std::cerr << sun << " " << score << " " << wait << std::endl;
	int nb_trees = info.trees_size[0][0] + info.trees_size[0][1] + info.trees_size[0][2] + info.trees_size[0][3]
		+ info.trees_size[1][0] + info.trees_size[1][1] + info.trees_size[1][2] + info.trees_size[1][3];
	std::cerr << nb_trees << std::endl;
	for (int i = 0; i < 37; i++) {
		if (!map[i].empty) {
			int size = map[i].size;
			int isMine = !map[i].player;
			int sleep = map[i].sleep;
			std::cerr << i << " " << size << " " << isMine << " " << sleep << std::endl;
		}
	}
	std::cerr << possible_moves.size() << std::endl;
	for (std::string& s : possible_moves) {
		std::cerr << s << std::endl;
	}

    for (int p = 0; p < 2; p++) {
        for (int i = 0; i < 4 ; i++) {
            std::cerr << info.trees_size[p][i] << ", ";
        }
        std::cerr << std::endl;
    }
}


double  Tree::simulate(bool disp) {
    State simulation(state);

    while (1) {
        if (simulation.final_state()) {
            if (disp) {
                print_map(simulation.grid, simulation.info, {{"s"}});
            }
            if (state.info.player) {
                return 1 - simulation.eval_state();
            } else {
                return simulation.eval_state();
            }
        }
        simulation.do_action(best_actionH(simulation));
		//simulation.do_action({Action_type::wait, 0, 0, 0});
    }
    return 0;
}

Action  Tree::best_action() const {
    double     best_score = 100000;
    int     index = 0;
    int     best_index = 0;
    
    //std::cerr << child.size() << std::endl;
	for (const Tree& t : child) {
        double score = t.score();
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
        // if (i == 0) {
        //     if (random_index(0, 20) == 3) {
        //         double s2 = leaf->simulate(true);
        //         std::cout << "PLAYER = " << s.info.player << std::endl;;
        //         std::cout << "score = " << s2 << " // " << score << std::endl;
        //         exit (0);
        //     }
        // }
        leaf->back_track(score);
        const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        i++;
    } while (time < 40000);

    //std::cerr << i << std::endl;

    // std::cerr << " BEST ACTION " << std::endl;
    // Action a = best_actionH(s);
    // Action b = root.best_action();

    // if (a.type != b.type) {
    //     std::cerr << "H :" << a << std::endl;
    //     std::cerr << "B :" << b << std::endl;
    //     root.print();
    //     exit (0);
    // }

    return root.best_action();
}
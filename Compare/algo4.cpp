#include <iostream>
#include <unistd.h>
#include <chrono>
#include <random>
#include <array>
#include <sstream>
#include <algorithm>
#include <functional>
#include <fstream>

#include "common.hpp"
#include "algo4.hpp"

namespace algo4 {

double	eval_state(State& s) {
	//std::cerr << static_cast<int>(info.days) << ", " << info.player << std::endl;
    if (s.final_state()) {
        double diff_score = s.info.score[0] - s.info.score[1];
        double score = diff_score / 100.0 + 0.5;
        return score;
    } else {
        double score1 = s.info.score[0] * (20 - s.info.nutriments) / 10.0 + s.info.trees_size[0][1] + s.info.trees_size[0][2] * 2 + s.info.trees_size[0][3] * 5;
        double score2 = s.info.score[1] * (20 - s.info.nutriments) / 10.0 + s.info.trees_size[1][1] + s.info.trees_size[1][2] * 2 + s.info.trees_size[1][3] * 5;
        double diff_score = (score1 - score2) / 50;
        double score = diff_score;

        return score;
    }
}
double  score_seed(State& s, Action& a, int player) {
    double score;

    Cell& c = s.grid[a.to];
    Cell& from = s.grid[a.from];
    if (c.richness == 2 || c.richness == 1) {
        score = c.richness == 2 ? 1.8 : 0.8;
        if (s.info.trees_richness[player][2] >= 1 && s.info.trees_richness[player][1] < 3 && (c.richness == 2 || s.info.trees_richness[player][0] < 3) && from.size > 1) {
            score += 0.1 * (6 - adjacent_trees(s, a.to, player)); // adjacent_trees returns the number of trees (you own) surrounding a cell
        }
    } else {
        score = s.info.trees_richness[player][2] < 4 ? 2 : 0;
        score -= (s.info.days < 8 ? 0.6 : 0.6 ) * adjacent_trees(s, a.to, player);
        score += from.size == 3 ? 0.1 : 0; // sow_size is the size of the tree in the cell sow
    }
    if (inSecondLayer(a.from, a.to)) // is the cell id in the second layer of neighbors of cell sow ?
       score += 0.1;
    if (isDiagonal(a.from, a.to)) // is the cell id diagonal to the cell sow ?
        score += 0.2;
    if (s.info.nutriments < 16)
        score -= 2;

    a.score = score;
    return score;
}
double  score_grow(State& s, Action& a, int player) {
    double score;

    Cell& c = s.grid[a.from];
    if (c.richness == 1)
        score = 10;
    else if (c.richness == 2)
        score = 5;
    else
        score = 6;
    if (shadow_next_turn(s, s.info.days, a.from))
        score -= 1;

    score += 0.1 * (c.size + 1);

    a.score = score;
    return score;
}
double  score_complete(State& s, Action& a, int player) {
    double score;

    Cell& c = s.grid[a.from];
    if (c.richness == 1)
        score = 0;
    else {
        score = c.richness;
    }
    if (s.info.trees_size[player][3] + 8 > s.info.nutriments)
        score += 21;
    if (s.info.days > 21)
        score += 21;
    if (!shadow_next_turn(s, s.info.days, a.from))
        score -= 2;
    a.score = score;
    return score;
}
Action  bestActionH(State& s) {
	int player = s.info.player;
    double best_score = 1.5;
    Action best_action;
    unsigned char prix;

    best_action = {Action_type::wait, 0, 0, 0};
	if (s.info.wait[player])
		return best_action;
    for (unsigned char i = 0; i < 37; i++) {
        if (!s.grid[i].empty and s.grid[i].player == player and !s.grid[i].sleep) {
            switch (s.grid[i].size)
            {
                case 0: {
                    prix = 1 + s.info.trees_size[player][1];
                    if (prix <= s.info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(s, a, player);
                        if (score > best_score) {
                            best_action = a;
                        }
                        best_score = score > best_score ? score : best_score;
                    }
                    break;
                }
                case 1: {
                    prix = 3 + s.info.trees_size[player][2];
                    if (prix <= s.info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(s, a, player);
                        if (score > best_score) {
                            best_action = a;
                        }
                        best_score = score > best_score ? score : best_score;
                    }
                    prix = s.info.trees_size[player][0];
                    if (prix > s.info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[0]; j++) {
                        int to = n[i].neigbours[j];
                        if (s.grid[to].empty and s.grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(s, a, player);
                            if (score > best_score) {
                                best_action = a;
                            }
                            best_score = score > best_score ? score : best_score;
                        }
                    }
                    break;
                }
                case 2: {
                    prix = 7 + s.info.trees_size[player][3];
                    if (prix <= s.info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(s, a, player);
                        if (score > best_score) {
                            best_action = a;
                        }
                        best_score = score > best_score ? score : best_score;
                    }                
                    prix = s.info.trees_size[player][0];
                    if (prix > s.info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[1]; j++) {
                        int to = n[i].neigbours[j];
                        if (s.grid[to].empty and s.grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(s, a, player);
                            if (score > best_score) {
                                best_action = a;
                            }
                            best_score = score > best_score ? score : best_score;
                        }
                    }
                    break;
                }
                case 3: {
                    if (s.info.sun[player] >= 4) {
                        Action a(Action_type::complete, i, 0, 4);
                        double score = score_complete(s, a, player);
                        if (score > best_score) {
                            best_action = a;
                        }
                        best_score = score > best_score ? score : best_score;
                    }
                    prix = s.info.trees_size[player][0];
                    if (prix > s.info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[2]; j++) {
                        int to = n[i].neigbours[j];
                        if (s.grid[to].empty and s.grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(s, a, player);
                            if (score > best_score) {
                                best_action = a;
                            }
                            best_score = score > best_score ? score : best_score;
                        }
                    }
                    break;
                }
                default: {
                    std::cerr << "Error in parser" << std::endl;
                    break;
                }
            }
        }
    }
    return best_action;
}
void	generate_all_actions(State& s, std::vector<Action>& actions) {
    unsigned char prix;
	int player = s.info.player;
    double score_min = 1.5;

    int nb_actions = 1;
	if (s.final_state())
		return ;
    Action a(Action_type::wait, 0, 0, 0);
    a.score = 1.4;
    actions.push_back(a);
	if (s.info.wait[player])
		return ;
    for (unsigned char i = 0; i < 37; i++) {
        if (nb_actions > 5)
            break;
        if (!s.grid[i].empty and s.grid[i].player == player and !s.grid[i].sleep) {
            switch (s.grid[i].size)
            {
                case 0: {
                    prix = 1 + s.info.trees_size[player][1];
                    if (prix <= s.info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(s, a, player);
                        if (score >= score_min) {
                            actions.push_back(a);
                            nb_actions++;
                        }
                    }
                    break;
                }
                case 1: {
                    prix = 3 + s.info.trees_size[player][2];
                    if (prix <= s.info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(s, a, player);
                        if (score >= score_min) {
                            actions.push_back(a);
                            nb_actions++;
                        }
                    }
                    prix = s.info.trees_size[player][0];
                    if (prix > s.info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[0]; j++) {
                        int to = n[i].neigbours[j];
                        if (s.grid[to].empty and s.grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(s, a, player);
                            if (score >= score_min) {
                                actions.push_back(a);
                                nb_actions++;
                            }
                        }
                    }
                    break;
                }
                case 2: {
                    prix = 7 + s.info.trees_size[player][3];
                    if (prix <= s.info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(s, a, player);
                        if (score >= score_min) {
                            actions.push_back(a);
                            nb_actions++;
                        }
                    }                
                    prix = s.info.trees_size[player][0];
                    if (prix > s.info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[1]; j++) {
                        int to = n[i].neigbours[j];
                        if (s.grid[to].empty and s.grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(s, a, player);
                            if (score >= score_min) {
                                actions.push_back(a);
                                nb_actions++;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    if (s.info.sun[player] >= 4) {
                        Action a(Action_type::complete, i, 0, 4);
                        double score = score_complete(s, a, player);
                        if (score >= score_min) {
                            actions.push_back(a);
                            nb_actions++;
                        }
                    }
                    prix = s.info.trees_size[player][0];
                    if (prix > s.info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[2]; j++) {
                        int to = n[i].neigbours[j];
                        if (s.grid[to].empty and s.grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(s, a, player);
                            if (score >= score_min) {
                                actions.push_back(a);
                                nb_actions++;
                            }
                        }
                    }
                    break;
                }
                default: {
                    std::cerr << "Error in parser" << std::endl;
                    break;
                }
            }
        }
    }
}
void	generate_all_actions_no_score(State& s, std::vector<Action>& actions) {
    unsigned char prix;
	int player = s.info.player;

	if (s.final_state())
		return ;

    actions.emplace_back(Action_type::wait, 0, 0, 0);
	if (s.info.wait[player])
		return ;
    for (unsigned char i = 0; i < 37; i++) {
        if (!s.grid[i].empty and s.grid[i].player == player and !s.grid[i].sleep) {
            switch (s.grid[i].size)
            {
                case 0: {
                    prix = 1 + s.info.trees_size[player][1];
                    if (prix <= s.info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        actions.push_back(a);
                    }
                    break;
                }
                case 1: {
                    prix = 3 + s.info.trees_size[player][2];
                    if (prix <= s.info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        actions.push_back(a);
                    }
                    prix = s.info.trees_size[player][0];
                    if (prix > s.info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[0]; j++) {
                        int to = n[i].neigbours[j];
                        if (s.grid[to].empty and s.grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            actions.push_back(a);
                        }
                    }
                    break;
                }
                case 2: {
                    prix = 7 + s.info.trees_size[player][3];
                    if (prix <= s.info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        actions.push_back(a);
                    }                
                    prix = s.info.trees_size[player][0];
                    if (prix > s.info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[1]; j++) {
                        int to = n[i].neigbours[j];
                        if (s.grid[to].empty and s.grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            actions.push_back(a);
                        }
                    }
                    break;
                }
                case 3: {
                    if (s.info.sun[player] >= 4) {
                        Action a(Action_type::complete, i, 0, 4);
                        actions.push_back(a);
                    }
                    prix = s.info.trees_size[player][0];
                    if (prix > s.info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[2]; j++) {
                        int to = n[i].neigbours[j];
                        if (s.grid[to].empty and s.grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            actions.push_back(a);
                        }
                    }
                    break;
                }
                default: {
                    std::cerr << "Error in parser" << std::endl;
                    break;
                }
            }
        }
    }
}
Action	random_action(State& s) {
	std::vector<Action>	actions;

	generate_all_actions(s, actions);
	return actions[random_index(0, actions.size() - 1)];
}
Action  best_actionH(State &s) {
    std::vector<Action> actions;
    generate_all_actions_no_score(s, actions);

    double score_max = -10;
    int index_max = 0;

    for (int i = 0; i < actions.size(); i++) {
        double score = score_action(s, actions[i]);
        if (score > score_max) {
            score_max = score;
            index_max = i;
        }
    }
    return actions[index_max];
}

Tree::Tree(const State& s, Tree *f) : father(f), state(s), w(0), n(0) {}
Tree    *Tree::select_tree() {
    double best_score = -10;
    Tree *best_tree = NULL;

    if (child.size() == 0)
        return this;
    
    int i = 0;
    for (Tree& t : child) {
        if (t.n == 0) {
            return t.select_tree();
        }
        double score = (1 - t.score()) + sqrt(2) * sqrt(log2(n) / t.n);
        if (score > best_score) {
            best_score = score;
            best_tree = &t;
        }
        i++;
    }
    return best_tree->select_tree();
}
void    Tree::expand() {
	generate_all_actions(state, actions);
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
    int i = 0;

    while (i < 20) {
        if (simulation.final_state()) {
            if (disp) {
                print_map(simulation.grid, simulation.info, {{"s"}});
            }
            if (state.info.player) {
                return 1 - eval_state(simulation);
            } else {
                return eval_state(simulation);
            }
        }
        //simulation.do_action(simulation.bestActionH());
        simulation.do_action(random_action(simulation));
		//simulation.do_action({Action_type::wait, 0, 0, 0});
        i++;
    }
    if (state.info.player) {
        return 1 - eval_state(simulation);
    } else {
        return eval_state(simulation);
    }
}
Action  Tree::best_action() const {
    int     best_score = -10;
    int     index = 0;
    int     best_index = 0;
    
	for (const Tree& t : child) {
        int score = t.nb_exploration();
		//std::cerr << "Score " << score << std::endl;
        if (score > best_score) {
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
    static double sum = 0;
    static double iter = 0;

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
    } while (time < 90000);
    return root.best_action();
}

}
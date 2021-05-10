#include <iostream>
#include <unistd.h>
#include <chrono>
#include <random>
#include <array>
#include <sstream>
#include <algorithm>
#include <functional>
#include <fstream>


struct Neigbours {
    std::array<int , 3> size;
    std::array<unsigned char , 36> neigbours;

    Neigbours(const std::array<int , 3>& s, const std::array<unsigned char , 36>& n) : size(s), neigbours(n) {};
};
const Neigbours n[37] = {
        { {6, 18, 36}, {1, 2, 3, 4, 5, 6, 7, 8, 18, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 36, 21, 35, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34} },
        { {6, 18, 29}, {7, 8, 2, 0, 6, 18, 19, 20, 36, 21, 9, 10, 3, 4, 5, 16, 17, 35, 22, 23, 24, 11, 12, 13, 14, 15, 32, 33, 34,} },
        { {6, 18, 29}, {8, 9, 10, 3, 0, 1, 20, 21, 7, 22, 23, 24, 11, 12, 4, 5, 6, 18, 19, 36, 25, 26, 27, 13, 14, 15, 16, 17, 35,} },
        { {6, 18, 29}, {2, 10, 11, 12, 4, 0, 8, 9, 1, 23, 24, 25, 26, 27, 13, 14, 5, 6, 20, 21, 7, 22, 18, 28, 29, 30, 15, 16, 17,} },
        { {6, 18, 29}, {0, 3, 12, 13, 14, 5, 1, 2, 6, 10, 11, 26, 27, 28, 29, 30, 15, 16, 7, 8, 18, 9, 17, 23, 24, 25, 31, 32, 33,} },
        { {6, 18, 29}, {6, 0, 4, 14, 15, 16, 18, 1, 17, 2, 3, 12, 13, 29, 30, 31, 32, 33, 36, 7, 35, 8, 34, 9, 10, 11, 26, 27, 28,} },
        { {6, 18, 29}, {18, 1, 0, 5, 16, 17, 36, 7, 35, 8, 2, 3, 4, 14, 15, 32, 33, 34, 19, 20, 21, 9, 10, 11, 12, 13, 29, 30, 31,} },
        { {6, 13, 22}, {19, 20, 8, 1, 18, 36, 21, 9, 2, 0, 6, 17, 35, 22, 23, 10, 3, 4, 5, 16, 33, 34} },
        { {6, 15, 23}, {20, 21, 9, 2, 1, 7, 19, 22, 23, 10, 3, 0, 6, 18, 36, 24, 11, 12, 4, 5, 16, 17, 35} },
        { {6, 13, 22}, {21, 22, 23, 10, 2, 8, 20, 24, 11, 3, 0, 1, 7, 19, 25, 26, 12, 4, 5, 6, 18, 36} },
        { {6, 15, 23}, {9, 23, 24, 11, 3, 2, 21, 22, 8, 25, 26, 12, 4, 0, 1, 20, 7, 27, 13, 14, 5, 6, 18} },
        { {6, 13, 22}, {10, 24, 25, 26, 12, 3, 9, 23, 2, 27, 13, 4, 0, 21, 22, 8, 1, 28, 29, 14, 5, 6} },
        { {6, 15, 23}, {3, 11, 26, 27, 13, 4, 2, 10, 0, 24, 25, 28, 29, 14, 5, 8, 9, 1, 23, 6, 30, 15, 16} },
        { {6, 13, 22}, {4, 12, 27, 28, 29, 14, 0, 3, 5, 11, 26, 30, 15, 1, 2, 6, 10, 16, 24, 25, 31, 32} },
        { {6, 15, 23}, {5, 4, 13, 29, 30, 15, 6, 0, 16, 3, 12, 27, 28, 31, 32, 18, 1, 17, 2, 33, 10, 11, 26} },
        { {6, 13, 22}, {16, 5, 14, 30, 31, 32, 17, 6, 33, 0, 4, 13, 29, 35, 18, 34, 1, 2, 3, 12, 27, 28} },
        { {6, 15, 23}, {17, 6, 5, 15, 32, 33, 35, 18, 34, 1, 0, 4, 14, 30, 31, 36, 7, 8, 2, 3, 12, 13, 29} },
        { {6, 13, 22}, {35, 18, 6, 16, 33, 34, 36, 7, 1, 0, 5, 15, 32, 19, 20, 8, 2, 3, 4, 14, 30, 31} },
        { {6, 15, 23}, {36, 7, 1, 6, 17, 35, 19, 20, 8, 2, 0, 5, 16, 33, 34, 21, 9, 10, 3, 4, 14, 15, 32} },
        { {3, 8, 15}, {20, 7, 36, 21, 8, 1, 18, 35, 22, 9, 2, 0, 6, 17, 34} },
        { {4, 10, 17}, {21, 8, 7, 19, 22, 9, 2, 1, 18, 36, 23, 10, 3, 0, 6, 17, 35} },
        { {4, 10, 17}, {22, 9, 8, 20, 23, 10, 2, 1, 7, 19, 24, 11, 3, 0, 6, 18, 36} },
        { {3, 8, 15}, {23, 9, 21, 24, 10, 2, 8, 20, 25, 11, 3, 0, 1, 7, 19} },
        { {4, 10, 17}, {22, 24, 10, 9, 21, 25, 11, 3, 2, 8, 20, 26, 12, 4, 0, 1, 7} },
        { {4, 10, 17}, {23, 25, 11, 10, 22, 9, 26, 12, 3, 2, 21, 8, 27, 13, 4, 0, 1} },
        { {3, 8, 15}, {24, 26, 11, 23, 10, 27, 12, 3, 22, 9, 2, 28, 13, 4, 0} },
        { {4, 10, 17}, {11, 25, 27, 12, 10, 24, 3, 28, 13, 4, 9, 23, 2, 0, 29, 14, 5} },
        { {4, 10, 17}, {12, 26, 28, 13, 3, 11, 4, 25, 29, 14, 2, 10, 0, 24, 5, 30, 15} },
        { {3, 8, 15}, {13, 27, 29, 4, 12, 14, 26, 30, 0, 3, 5, 11, 15, 25, 31} },
        { {4, 10, 17}, {14, 13, 28, 30, 5, 4, 15, 12, 27, 31, 6, 0, 16, 3, 32, 11, 26} },
        { {4, 10, 17}, {15, 14, 29, 31, 16, 5, 32, 4, 13, 28, 17, 6, 33, 0, 3, 12, 27} },
        { {3, 8, 15}, {32, 15, 30, 33, 16, 5, 14, 29, 34, 17, 6, 0, 4, 13, 28} },
        { {4, 10, 17}, {33, 16, 15, 31, 34, 17, 6, 5, 14, 30, 35, 18, 1, 0, 4, 13, 29} },
        { {4, 10, 17}, {34, 17, 16, 32, 35, 18, 6, 5, 15, 31, 36, 7, 1, 0, 4, 14, 30} },
        { {3, 8, 15}, {35, 17, 33, 36, 18, 6, 16, 32, 19, 7, 1, 0, 5, 15, 31} },
        { {4, 10, 17}, {36, 18, 17, 34, 19, 7, 1, 6, 16, 33, 20, 8, 2, 0, 5, 15, 32} },
        { {4, 10, 17}, {19, 7, 18, 35, 20, 8, 1, 6, 17, 34, 21, 9, 2, 0, 5, 16, 33} },
    };
struct Shadow {
    std::array<std::array<int, 3>, 37> shadow;
    Shadow(const std::array<std::array<int, 3>, 37>& s) : shadow(s) {};
};
Shadow s[6] = {
{{{ {{4, 13, 28, }}, {{0, 4, 13, }}, {{3, 12, 27, }}, {{12, 27, -1, }}, {{13, 28, -1, }}, {{14, 29, -1, }}, {{5, 14, 29, }}, {{1, 0, 4, }}, {{2, 3, 12, }}, {{10, 11, 26, }}, {{11, 26, -1, }}, {{26, -1, -1, }}, {{27, -1, -1, }}, {{28, -1, -1, }}, {{29, -1, -1, }}, {{30, -1, -1, }}, {{15, 30, -1, }}, {{16, 15, 30, }}, {{6, 5, 14, }}, {{7, 1, 0, }}, {{8, 2, 3, }}, {{9, 10, 11, }}, {{23, 24, 25, }}, {{24, 25, -1, }}, {{25, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{31, -1, -1, }}, {{32, 31, -1, }}, {{33, 32, 31, }}, {{17, 16, 15, }}, {{18, 6, 5, }},   }}},
{{{ {{5, 15, 31, }}, {{6, 16, 32, }}, {{0, 5, 15, }}, {{4, 14, 30, }}, {{14, 30, -1, }}, {{15, 31, -1, }}, {{16, 32, -1, }}, {{18, 17, 33, }}, {{1, 6, 16, }}, {{2, 0, 5, }}, {{3, 4, 14, }}, {{12, 13, 29, }}, {{13, 29, -1, }}, {{29, -1, -1, }}, {{30, -1, -1, }}, {{31, -1, -1, }}, {{32, -1, -1, }}, {{33, -1, -1, }}, {{17, 33, -1, }}, {{36, 35, 34, }}, {{7, 18, 17, }}, {{8, 1, 6, }}, {{9, 2, 0, }}, {{10, 3, 4, }}, {{11, 12, 13, }}, {{26, 27, 28, }}, {{27, 28, -1, }}, {{28, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{34, -1, -1, }}, {{35, 34, -1, }},   }}},
{{{ {{6, 17, 34, }}, {{18, 35, -1, }}, {{1, 18, 35, }}, {{0, 6, 17, }}, {{5, 16, 33, }}, {{16, 33, -1, }}, {{17, 34, -1, }}, {{36, -1, -1, }}, {{7, 36, -1, }}, {{8, 7, 36, }}, {{2, 1, 18, }}, {{3, 0, 6, }}, {{4, 5, 16, }}, {{14, 15, 32, }}, {{15, 32, -1, }}, {{32, -1, -1, }}, {{33, -1, -1, }}, {{34, -1, -1, }}, {{35, -1, -1, }}, {{-1, -1, -1, }}, {{19, -1, -1, }}, {{20, 19, -1, }}, {{21, 20, 19, }}, {{9, 8, 7, }}, {{10, 2, 1, }}, {{11, 3, 0, }}, {{12, 4, 5, }}, {{13, 14, 15, }}, {{29, 30, 31, }}, {{30, 31, -1, }}, {{31, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }},   }}},
{{{ {{1, 7, 19, }}, {{7, 19, -1, }}, {{8, 20, -1, }}, {{2, 8, 20, }}, {{0, 1, 7, }}, {{6, 18, 36, }}, {{18, 36, -1, }}, {{19, -1, -1, }}, {{20, -1, -1, }}, {{21, -1, -1, }}, {{9, 21, -1, }}, {{10, 9, 21, }}, {{3, 2, 8, }}, {{4, 0, 1, }}, {{5, 6, 18, }}, {{16, 17, 35, }}, {{17, 35, -1, }}, {{35, -1, -1, }}, {{36, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{22, -1, -1, }}, {{23, 22, -1, }}, {{24, 23, 22, }}, {{11, 10, 9, }}, {{12, 3, 2, }}, {{13, 4, 0, }}, {{14, 5, 6, }}, {{15, 16, 17, }}, {{32, 33, 34, }}, {{33, 34, -1, }}, {{34, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }},   }}},
{{{ {{2, 9, 22, }}, {{8, 21, -1, }}, {{9, 22, -1, }}, {{10, 23, -1, }}, {{3, 10, 23, }}, {{0, 2, 9, }}, {{1, 8, 21, }}, {{20, -1, -1, }}, {{21, -1, -1, }}, {{22, -1, -1, }}, {{23, -1, -1, }}, {{24, -1, -1, }}, {{11, 24, -1, }}, {{12, 11, 24, }}, {{4, 3, 10, }}, {{5, 0, 2, }}, {{6, 1, 8, }}, {{18, 7, 20, }}, {{7, 20, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{25, -1, -1, }}, {{26, 25, -1, }}, {{27, 26, 25, }}, {{13, 12, 11, }}, {{14, 4, 3, }}, {{15, 5, 0, }}, {{16, 6, 1, }}, {{17, 18, 7, }}, {{35, 36, 19, }}, {{36, 19, -1, }}, {{19, -1, -1, }},   }}},
{{{ {{3, 11, 25, }}, {{2, 10, 24, }}, {{10, 24, -1, }}, {{11, 25, -1, }}, {{12, 26, -1, }}, {{4, 12, 26, }}, {{0, 3, 11, }}, {{8, 9, 23, }}, {{9, 23, -1, }}, {{23, -1, -1, }}, {{24, -1, -1, }}, {{25, -1, -1, }}, {{26, -1, -1, }}, {{27, -1, -1, }}, {{13, 27, -1, }}, {{14, 13, 27, }}, {{5, 4, 12, }}, {{6, 0, 3, }}, {{1, 2, 10, }}, {{20, 21, 22, }}, {{21, 22, -1, }}, {{22, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{-1, -1, -1, }}, {{28, -1, -1, }}, {{29, 28, -1, }}, {{30, 29, 28, }}, {{15, 14, 13, }}, {{16, 5, 4, }}, {{17, 6, 0, }}, {{18, 1, 2, }}, {{7, 8, 9, }},   }}},
};

std::mt19937 gen(std::random_device{}());
int random_index(int lower, int higher) {
    return std::uniform_int_distribution<int>(lower, higher)(gen);
}

struct Cell {
	bool			player : 1; // player of ?
	bool			sleep : 1; // sleeping
	bool			empty : 1; // vide or not
	unsigned char	size : 2; // 0 to 3
	unsigned char	richness : 2; // 0 to 3
	bool			info : 1; // info supplementaire bool dispo
}; // TAILLE TOTAL 1 Octet = taille char
struct	Info {
	bool			player;
	unsigned char	days; // 0 - 64 Les jours ne vont que a 24
	int				nutriments;

	std::array<bool, 2>					wait;
	std::array<short, 2>				score; // 0 - 4096
	std::array<short, 2>				sun; // 0 - 4096
    std::array<std::array<int, 4>, 2> 	trees_size;
}; // TAILLE TOTAL 8 Octet = taille long long int

//std::function<bool(State&, Action, int, int)> t[4] = {seed_action, grow_action, complete_action, wait_action};
enum struct Action_type {
	seed,
	grow,
	complete,
	wait,
	none,
};

std::ostream&   operator<<(std::ostream& f, Action_type a) {
    switch (a)
    {
    case Action_type::seed :
        f << "SEED";
        break;
    case Action_type::grow :
        f << "GROW";
        break;
    case Action_type::complete :
        f << "COMPLETE";
        break;
    case Action_type::wait :
        f << "WAIT";
        break;
    default:
        f << "Error in parser" << std::endl;
        break;
    }
    return f;
}

struct Action {
    Action_type		type; // 1 1 1 => 7
	unsigned char   from : 6; // index from
	unsigned char   to : 6; // index to for seed
    unsigned char   cost : 6; 
    // Il reste 4 bytes

	Action() {}
    Action(Action_type ty, unsigned char f, unsigned char t, unsigned char c) : type(ty), from(f), to(t), cost(c) {}
}; // TAILLE TOTAL 4 Octet = taille of int


std::ostream&   operator<<(std::ostream& f, Action a) {
    switch (a.type)
    {
    case Action_type::seed :
        f << a.type << " " << static_cast<int>(a.from) << " " << static_cast<int>(a.to);
        break;
    case Action_type::grow :
        f << a.type << " " << static_cast<int>(a.from);
        break;
    case Action_type::complete :
        f << a.type << " " << static_cast<int>(a.from);
        break;
    case Action_type::wait :
        f << a.type;
        break;
    default:
        f << "Error in parser";
        break;
    }
    return f;
}

struct State {
    Info                    info;
    std::array<Cell, 37>    grid;
    Action                  a;

	State() {}
	State(const Info& i, const std::array<Cell, 37>& g) : info(i), grid(g) {}
	void	generate_all_actions(std::vector<Action>& actions);
	Action	random_action();
	void	simulate_action(State& new_s, Action a);
	void	do_action(Action a);
	bool	final_state();
	double	eval_state();
	void	print_map();
};

void	State::generate_all_actions(std::vector<Action>& actions) {
    unsigned char prix;
	int player = info.player;

	if (final_state())
		return ;
    actions.emplace_back(Action_type::wait, 0, 0, 0);
	if (info.wait[player])
		return ;
    for (unsigned char i = 0; i < 37; i++) {
        if (!grid[i].empty and grid[i].player == player and !grid[i].sleep) {
            switch (grid[i].size)
            {
            case 0:
                prix = 1 + info.trees_size[player][1];
                if (prix <= info.sun[player]) {
                    actions.emplace_back(Action_type::grow, i, 0, prix);
                }
                break;
            case 1:
                prix = 3 + info.trees_size[player][2];
                if (prix <= info.sun[player]) {
                    actions.emplace_back(Action_type::grow, i, 0, prix);
                }
                prix = info.trees_size[player][0];
                if (prix > info.sun[player])
                    break;
                for (int j = 0; j < n[i].size[0]; j++) {
                    int to = n[i].neigbours[j];
                    if (grid[to].empty and grid[to].richness != 0) {
                        actions.emplace_back(Action_type::seed, i, to, prix);
                    }
                }
                break;
            case 2:
                prix = 7 + info.trees_size[player][3];
                if (prix <= info.sun[player]) {
                    actions.emplace_back(Action_type::grow, i, 0, prix);
                }                
                prix = info.trees_size[player][0];
                if (prix > info.sun[player])
                    break;
                for (int j = 0; j < n[i].size[1]; j++) {
                    int to = n[i].neigbours[j];
                    if (grid[to].empty and grid[to].richness != 0) {
                        actions.emplace_back(Action_type::seed, i, to, prix);
                    }
                }
                break;
            case 3:
                if (info.sun[player] >= 4)
                    actions.emplace_back(Action_type::complete, i, 0, 4);
                prix = info.trees_size[player][0];
                if (prix > info.sun[player])
                    break;
                for (int j = 0; j < n[i].size[2]; j++) {
                    int to = n[i].neigbours[j];
                    if (grid[to].empty and grid[to].richness != 0) {
                        actions.emplace_back(Action_type::seed, i, to, prix);
                    }
                }
                break;
            default:
                std::cerr << "Error in parser" << std::endl;
                break;
            }
        }
    }
}

bool	State::final_state() {
	if (info.days >= 24)
		return true;
	return false;
}

double	State::eval_state() {
	//std::cerr << static_cast<int>(info.days) << ", " << info.player << std::endl;

    double score = 1 / (1 + exp(info.score[0] - info.score[1]));

    return score;

	// if (info.score[0] > info.score[1]) {
    //     return 1;
	// }
	// if (info.score[0] == info.score[1])
	// 	return 0.5;
	// if (info.player)
	// 	return 1;
	// return 0;
}

Action	State::random_action() {
	std::vector<Action>	actions;

	generate_all_actions(actions);
	return actions[random_index(0, actions.size() - 1)];
}

void	pass_day(State &state) {
	state.info.days++;
	if (state.info.days >= 24)
		return ;
	int orientaion = state.info.days % 6;

	state.info.wait[0] = false;
	state.info.wait[1] = false;
	for (int i = 0; i < state.grid.size(); i++) {
		int size = state.grid[i].size;
		state.grid[i].sleep = false;
		if (state.grid[i].empty or size == 0)
			continue;
		
		bool shadow = false;
		for (int j = 0; j < 3; j++) {
			int index_cell = s[orientaion].shadow[i][j];
			if (index_cell == -1 or state.grid[index_cell].empty) {
				continue;
			}

			if (state.grid[index_cell].size < size or j >= state.grid[index_cell].size) {
				continue;
			}
			shadow = true;
			break;
		}
		if (!shadow) {
			state.info.sun[state.grid[i].player] += size;
		}
	}

}
bool	seed_action(State &s, Action a, int player, int info) {
	(void)info;
    s.grid[a.from].sleep = true;
	if (!s.grid[a.to].empty) {
		s.grid[a.to].empty = true;
		return true;
	}
	s.grid[a.to].empty = false;
	s.grid[a.to].player = player;
	s.grid[a.to].size = 0;
	s.grid[a.to].sleep = true;
	s.info.trees_size[player][0]++;
	return false;
}
bool	complete_action(State &s, Action a, int player, int info) {
	s.grid[a.from].empty = true;
	s.info.trees_size[player][3]--;
	s.info.score[player] += (info + s.grid[a.from].richness);
	s.info.nutriments--;
	return false;
}
bool	grow_action(State &s, Action a, int player, int info) {
    s.grid[a.from].sleep = true;
	s.info.trees_size[player][s.grid[a.from].size]--;
	s.grid[a.from].size += 1;
	s.info.trees_size[player][s.grid[a.from].size]++;
	return false;
}
bool	wait_action(State &s, Action a, int player, int info) {
	s.info.wait[player] = true;
	return false;
}
std::function<bool(State&, Action, int, int)> t[4] = {seed_action, grow_action, complete_action, wait_action};
void	State::simulate_action(State& new_s, Action a) {
    new_s = *this;
	int nutriments = this->info.nutriments;

    if (!this->info.player) {
        new_s.a = a;
    } else {
        new_s.info.sun[0] -= this->a.cost;
        new_s.info.sun[1] -= a.cost;
		t[static_cast<int>(this->a.type)](new_s, this->a, 0, nutriments);
		
		if (t[static_cast<int>(a.type)](new_s, a, 1, nutriments)) {
			new_s.info.sun[0] += this->a.cost;
        	new_s.info.sun[1] += a.cost;
		}

		if (new_s.info.wait[0] and new_s.info.wait[1]) {
			pass_day(new_s);
		}
	}

	new_s.info.player = !this->info.player;
}
void	State::do_action(Action a) {
	int nutriments = this->info.nutriments;

    if (!this->info.player) {
        this->a = a;
    } else {
        this->info.sun[0] -= this->a.cost;
        this->info.sun[1] -= a.cost;
		t[static_cast<int>(this->a.type)](*this, this->a, 0, nutriments);
		
		if (t[static_cast<int>(a.type)](*this, a, 1, nutriments)) {
			this->info.sun[0] += this->a.cost;
        	this->info.sun[1] += a.cost;
		}

		if (this->info.wait[0] and this->info.wait[1]) {
			pass_day(*this);
		}
	}
	this->info.player = !this->info.player;
}
void    print_map(std::array<Cell, 37> map, Info info, std::vector<std::string> possible_moves) {
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
}


Action	extract_action(std::string str, int player) {
	Action a;
	char skip[20];
	int from, to;

	switch (str[0]) {
		case 'S' :
			sscanf(str.c_str(), "%s%d%d", skip, &from , &to);
			a.from = from;
			a.to = to;
			a.type = Action_type::seed;
			break;
		case 'G' :
			sscanf(str.c_str(), "%s%d", skip, &from);
			a.from = from;
			a.type = Action_type::grow;			
		break;
		case 'W' :
			a.type = Action_type::wait;
			break;
		case 'C' :
			sscanf(str.c_str(), "%s%d", skip, &from);
			a.from = from;
			a.type = Action_type::complete;
			break;
	}
	return a;
}

int		get_cost(State &s, Action a, int player) {
    switch (a.type)
    {
    case Action_type::seed :
		return (s.info.trees_size[player][0]);
    case Action_type::grow :
		switch (s.grid[a.from].size)
		{
		case 0:
			return (1 + s.info.trees_size[player][1]);
		case 1:
			return (3 + s.info.trees_size[player][2]);
		case 2:
			return (7 + s.info.trees_size[player][3]);
		}
    case Action_type::complete :
		return 4;
    case Action_type::wait :
		return 0;
    default:
		return 0;
    }
}

void	simulate_game(State &s) {
	std::ifstream	f("action.txt");
	std::string		l1;
	std::string		l2;

	while (std::getline(f, l1)) {
		std::getline(f, l2);
		Action a1 = extract_action(l1, 0);
		Action a2 = extract_action(l2, 0);

		a1.cost = get_cost(s, a1, 0);
		a2.cost = get_cost(s, a2, 1);

		s.do_action(a1);
		s.do_action(a2);

		//std::cin.ignore();
	}
	
	print_map(s.grid, s.info, {{""}});

	exit (0);
}

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
};

Tree::Tree(const State& s, Tree *f) : state(s), father(f), w(0), n(0) {}

Tree    *Tree::select_tree() {
    double best_score = -10;
    Tree *best_tree = NULL;

    if (child.size() == 0)
        return this;
    
    for (Tree& t : child) {
		if (t.get_player() == state.info.player) {
		}
        if (t.n == 0) {
            return t.select_tree();
        }
        double score = (1 - t.score()) + sqrt(2) * sqrt(log2(n) / t.n);
        if (score > best_score) {
            best_score = score;
            best_tree = &t;
        }
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
            return state.info.player - simulation.eval_state();
        }
        Action a = simulation.random_action();
		simulation.do_action(a);
    }
    return 0;
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

Action  best_moove(const State& s) {
    Tree root(s, NULL);
    root.expand();
	//std::cerr << "DEBUG " << root.child_size() << std::endl;
    const std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
    long long unsigned time;
    int i = 0;
    do {
		// std::cerr << "SELECT TREE" << std::endl;
        Tree *leaf = root.select_tree();
		// std::cerr << "END SELECT TREE" << std::endl;
        if (leaf->explore()) {
            leaf->expand();
            leaf = leaf->select_tree();
        }
        double score = leaf->simulate();
		// std::cerr << "SCORE = " << score << std::endl;
        leaf->back_track(score);
        const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        i++;
    } while (time < 10000000);
    std::cerr << i << std::endl;

    return root.best_action();
}


int     main(int argc, char **argv) {
    std::array<Cell, 37>    map;
    Info                    info;

	// std::streambuf *psbuf;
	// std::stringstream new_stream;

	// psbuf = new_stream.rdbuf();
	// std::cout.rdbuf(psbuf);

    int numberOfCells; // 37
    std::cin >> numberOfCells; std::cin.ignore();

    for (int i = 0; i < numberOfCells; i++) {
        int index, richness, neigh; 
        std::cin >> index >> richness >> neigh >> neigh >> neigh >> neigh >> neigh >> neigh; std::cin.ignore();
        map[index].richness = richness;
        map[index].empty = true;
    }
    while (1) {
		info.wait[0] = false;

        int day; // the game lasts 24 days: 0-23
        std::cin >> day; std::cin.ignore();
        int nutrients; // the base score you gain from the next COMPLETE action
        std::cin >> nutrients; std::cin.ignore();

        info.days = day;
        info.nutriments = nutrients;

        int sun;
        int score;

        std::cin >> sun >> score; std::cin.ignore();
        info.player = 0;
        info.sun[0] = sun;
        info.score[0] = score;

        int wait;
        std::cin >> sun >> score >> wait; std::cin.ignore();
        info.sun[1] = sun;
        info.score[1] = score;
        info.wait[1] = wait;

        int numberOfTrees;
        std::cin >> numberOfTrees; std::cin.ignore();

        info.trees_size[0].fill(0);
        info.trees_size[1].fill(0);
		//std::cout << "WTF " << numberOfTrees << std::endl;
		for (int i = 0; i < numberOfCells; i++) {
			map[i].empty = true;
		}
        for (int i = 0; i < numberOfTrees; i++) {
            int cellIndex, size;
            int isMine, isDormant;
            std::cin >> cellIndex >> size >> isMine >> isDormant; std::cin.ignore();
            map[cellIndex].size = size;
            map[cellIndex].player = !isMine;
            map[cellIndex].sleep = isDormant;
            map[cellIndex].empty = false;
            info.trees_size[0][size] += isMine;
            info.trees_size[1][size] += !isMine;
        }

        int numberOfPossibleMoves;
        std::cin >> numberOfPossibleMoves; std::cin.ignore();
        std::string 				possibleMove_ia;
        std::vector<std::string>	possibleMoves_ia;

        for (int i = 0; i < numberOfPossibleMoves; i++) {
            getline(std::cin, possibleMove_ia);
			possibleMoves_ia.emplace_back(possibleMove_ia);
            //std::cerr << "IA " << possibleMove_ia << std::endl;
        }
		State s(info, map);

        int i = 0;
        while (!s.final_state()) {
            Action a = best_moove(s);
            s.do_action(a);
            std::cout << "P1 " << a << std::endl;
            if (s.final_state())
                break;
            a = best_moove(s);
            std::cout << "P2 " << a << std::endl;
            s.do_action(a);
            i++;
        }

        std::cout << "TURN " << i << std::endl;
        std::cout << s.info.score[0] << std::endl;
        std::cout << s.info.score[1] << std::endl;

        std::cout << s.info.sun[0] << std::endl;
        std::cout << s.info.sun[1] << std::endl;


        std::cout << s.info.player << std::endl;

        return 0;
    }
}
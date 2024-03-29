#include <iostream>
#include <unistd.h>
#include <chrono>
#include <random>
#include <array>
#include <sstream>
#include <algorithm>
#include <functional>
#include <fstream>

namespace algo2 {


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
    std::array<std::array<int, 3>, 2> 	trees_richness;
}; // TAILLE TOTAL 8 Octet = taille long long int
//std::function<bool(State&, Action, int, int)> t[4] = {seed_action, grow_action, complete_action, wait_action};
enum struct Action_type {
	seed,
	grow,
	complete,
	wait,
	none,
};
struct Action {
    Action_type		type; // 1 1 1 => 7
	unsigned char   from; // index from
	unsigned char   to; // index to for seed
    unsigned char   cost;
    double          score;
    // Il reste 4 bytes

	Action() {}
    Action(Action_type ty, unsigned char f, unsigned char t, unsigned char c) : type(ty), from(f), to(t), cost(c) {}
}; // TAILLE TOTAL 4 Octet = taille of int
struct State {
    Info                    info;
    std::array<Cell, 37>    grid;
    Action                  a;

	State() {}
	State(const Info& i, const std::array<Cell, 37>& g) : info(i), grid(g) {}
	void	generate_all_actions(std::vector<Action>& actions);
    void	generate_all_actions_no_score(std::vector<Action>& actions) const;
    Action  bestActionH();

	Action	random_action();
	void	simulate_action(State& new_s, Action a);
	void	do_action(Action a);
	bool	final_state() const;
	double	eval_state() const;
	void	print_map();

    double  score_seed(Action& a, int player);
    double  score_grow(Action& a);
    double  score_complete(Action& a);
};


struct Diag {
    std::array<int , 3> size;
    std::array<bool , 36> diag;

    Diag(const std::array<int , 3>& s, const std::array<bool , 36>& d) : size(s), diag(d) {};
};

const Diag d[37] = { 
    { {6, 18, 36}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, } },
    { {6, 18, 29}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, } },
    { {6, 18, 29}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, } },
    { {6, 18, 29}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, } },
    { {6, 18, 29}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, } },
    { {6, 18, 29}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, } },
    { {6, 18, 29}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, } },
    { {6, 13, 22}, {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, } },
    { {6, 15, 23}, {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, } },
    { {6, 13, 22}, {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, } },
    { {6, 15, 23}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, } },
    { {6, 13, 22}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, } },
    { {6, 15, 23}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, } },
    { {6, 13, 22}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, } },
    { {6, 15, 23}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, } },
    { {6, 13, 22}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, } },
    { {6, 15, 23}, {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, } },
    { {6, 13, 22}, {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, } },
    { {6, 15, 23}, {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, } },
    { {3, 8, 15}, {0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, } },
    { {4, 10, 17}, {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, } },
    { {4, 10, 17}, {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, } },
    { {3, 8, 15}, {0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, } },
    { {4, 10, 17}, {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, } },
    { {4, 10, 17}, {0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, } },
    { {3, 8, 15}, {0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, } },
    { {4, 10, 17}, {0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, } },
    { {4, 10, 17}, {0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, } },
    { {3, 8, 15}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, } },
    { {4, 10, 17}, {0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, } },
    { {4, 10, 17}, {0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, } },
    { {3, 8, 15}, {0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, } },
    { {4, 10, 17}, {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, } },
    { {4, 10, 17}, {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, } },
    { {3, 8, 15}, {0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, } },
    { {4, 10, 17}, {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, } },
    { {4, 10, 17}, {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, } }
};
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

int     adjacent_trees(State& s, int index, int player) {
    int neigbours = 0;
    for (int i = 0; i < n[index].size[0]; i++) {
        if (!s.grid[n[index].neigbours[i]].empty and s.grid[n[index].neigbours[i]].player == player) {
            neigbours++;
        }
    }
    return neigbours;
}
bool    isDiagonal(int from, int to) {
    int start = n[from].size[0];
    int end = n[from].size[1];

    for (int i = start; i < end; i++) {
        if (n[from].neigbours[i] == to) {
            return d[from].diag[i];
        }
    }
    return false;
}
bool    inSecondLayer(int from, int to) {
    int start = n[from].size[0];
    int end = n[from].size[1];

    for (int i = start; i < end; i++) {
        if (n[from].neigbours[i] == to)
            return true;
    }
    return false;
}
bool    shadow_next_turn(State& state, int day, int id) {
    int orientaion = (day + 1) % 6;
    int size = state.grid[id].size;
    for (int j = 0; j < 3; j++) {
        int index_cell = s[orientaion].shadow[id][j];
        if (index_cell == -1 or state.grid[index_cell].empty) {
            continue;
        }
        if (state.grid[index_cell].size < size or j >= state.grid[index_cell].size) {
            continue;
        }
        return (true);
    }
    return false;
}
double  State::score_seed(Action& a, int player) {
    double score;

    Cell& c = grid[a.to];
    Cell& from = grid[a.from];
    if (c.richness == 2 || c.richness == 1) {
        score = c.richness == 2 ? 0.8 : 0.8;
        if (info.trees_richness[player][2] >= 1 && info.trees_richness[player][1] < 3 && (c.richness == 2 || info.trees_richness[player][0] < 3) && from.size > 1) {
            score += 0.1 * (6 - adjacent_trees(*this, a.to, player)); // adjacent_trees returns the number of trees (you own) surrounding a cell
        }
    } else {
        score = info.trees_richness[player][2] < 4 ? 2 : 0;
        score -= (info.days < 8 ? 0.6 : 0.6 ) * adjacent_trees(*this, a.to, player);
        score += from.size == 3 ? 0.1 : 0; // sow_size is the size of the tree in the cell sow
    }
    if (inSecondLayer(a.from, a.to)) // is the cell id in the second layer of neighbors of cell sow ?
        score += 0.1;
    if (isDiagonal(a.from, a.to)) // is the cell id diagonal to the cell sow ?
        score += 0.1;
    if (info.days > 18)
        score -= 2;

    a.score = score;
    return score;
}
double  State::score_grow(Action& a) {
    double score;

    Cell& c = grid[a.from];
    if (c.richness == 1)
        score = 4;
    else if (c.richness == 2)
        score = 2.5;
    else
        score = 3;
    if (shadow_next_turn(*this, info.days, a.from))
        score -= 1;
    score += 0.1 * (c.size + 1);

    a.score = score;
    return score;
}
double  State::score_complete(Action& a) {
    double score;
    
    Cell& c = grid[a.from];
    if (c.richness == 1)
        score = 0;
    else
        score = c.richness;
    if (info.days > 21)
        score += 21;
    if (!shadow_next_turn(*this, info.days, a.from))
        score -= 2;
    a.score = score;
    return score;
}

void	State::generate_all_actions(std::vector<Action>& actions) {
    unsigned char prix;
	int player = info.player;
    double score_min = 1.5;

    int nb_actions = 1;
	if (final_state())
		return ;
    Action a(Action_type::wait, 0, 0, 0);
    a.score = 1.4;
    actions.push_back(a);
	if (info.wait[player])
		return ;
    for (unsigned char i = 0; i < 37; i++) {
        if (nb_actions > 5)
            break;
        if (!grid[i].empty and grid[i].player == player and !grid[i].sleep) {
            switch (grid[i].size)
            {
                case 0: {
                    prix = 1 + info.trees_size[player][1];
                    if (prix <= info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(a);
                        if (score >= score_min) {
                            actions.push_back(a);
                            nb_actions++;
                        }
                    }
                    break;
                }
                case 1: {
                    prix = 3 + info.trees_size[player][2];
                    if (prix <= info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(a);
                        if (score >= score_min) {
                            actions.push_back(a);
                            nb_actions++;
                        }
                    }
                    prix = info.trees_size[player][0];
                    if (prix > info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[0]; j++) {
                        int to = n[i].neigbours[j];
                        if (grid[to].empty and grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(a, player);
                            if (score >= score_min) {
                                actions.push_back(a);
                                nb_actions++;
                            }
                        }
                    }
                    break;
                }
                case 2: {
                    prix = 7 + info.trees_size[player][3];
                    if (prix <= info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(a);
                        if (score >= score_min) {
                            actions.push_back(a);
                            nb_actions++;
                        }
                    }                
                    prix = info.trees_size[player][0];
                    if (prix > info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[1]; j++) {
                        int to = n[i].neigbours[j];
                        if (grid[to].empty and grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(a, player);
                            if (score >= score_min) {
                                actions.push_back(a);
                                nb_actions++;
                            }
                        }
                    }
                    break;
                }
                case 3: {
                    if (info.sun[player] >= 4) {
                        Action a(Action_type::complete, i, 0, 4);
                        double score = score_complete(a);
                        if (score >= score_min) {
                            actions.push_back(a);
                            nb_actions++;
                        }
                    }
                    prix = info.trees_size[player][0];
                    if (prix > info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[2]; j++) {
                        int to = n[i].neigbours[j];
                        if (grid[to].empty and grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(a, player);
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

void	State::generate_all_actions_no_score(std::vector<Action>& actions) const {
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
                case 0: {
                    prix = 1 + info.trees_size[player][1];
                    if (prix <= info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        actions.push_back(a);
                    }
                    break;
                }
                case 1: {
                    prix = 3 + info.trees_size[player][2];
                    if (prix <= info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        actions.push_back(a);
                    }
                    prix = info.trees_size[player][0];
                    if (prix > info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[0]; j++) {
                        int to = n[i].neigbours[j];
                        if (grid[to].empty and grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            actions.push_back(a);
                        }
                    }
                    break;
                }
                case 2: {
                    prix = 7 + info.trees_size[player][3];
                    if (prix <= info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        actions.push_back(a);
                    }                
                    prix = info.trees_size[player][0];
                    if (prix > info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[1]; j++) {
                        int to = n[i].neigbours[j];
                        if (grid[to].empty and grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            actions.push_back(a);
                        }
                    }
                    break;
                }
                case 3: {
                    if (info.sun[player] >= 4) {
                        Action a(Action_type::complete, i, 0, 4);
                        actions.push_back(a);
                    }
                    prix = info.trees_size[player][0];
                    if (prix > info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[2]; j++) {
                        int to = n[i].neigbours[j];
                        if (grid[to].empty and grid[to].richness != 0) {
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

bool	State::final_state() const {
	if (info.days >= 24)
		return true;
	return false;
}

double	State::eval_state() const {
	//std::cerr << static_cast<int>(info.days) << ", " << info.player << std::endl;


    if (final_state()) {
        double diff_score = info.score[0] - info.score[1];
        double score = diff_score / 100.0 + 0.5;
        return score;
    } else {
        double score1 = info.score[0] * (20 - info.nutriments) / 10.0 + info.trees_size[0][1] + info.trees_size[0][2] * 2 + info.trees_size[0][3] * 5;
        double score2 = info.score[1] * (20 - info.nutriments) / 10.0 + info.trees_size[1][1] + info.trees_size[1][2] * 2 + info.trees_size[1][3] * 5;
        double diff_score = (score1 - score2) / 50;
        double score = diff_score;

        return score;

    }

    //std::cerr << "Diff score " << diff_score << " : " << info.score[0] << " vs " << info.score[1] << std::endl;

    //double score = 1 / (1 + exp(-diff_score));


    //std::cerr << score << std::endl;


	// if (info.score[0] > info.score[1])
    //     return 1;
	// if (info.score[0] == info.score[1])
	// 	return 0.5;
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
    s.info.trees_richness[player][s.grid[a.to].richness - 1]++;

	return false;
}
bool	complete_action(State &s, Action a, int player, int info) {
	s.grid[a.from].empty = true;
	s.info.trees_size[player][3]--;

    //std::cerr << "COMPLETE !!!!! " << player << ", " << s.grid[a.from].richness - 1 << std::endl;
    //std::cerr <<  s.info.trees_richness[player][s.grid[a.from].richness - 1] << std::endl;

    s.info.trees_richness[player][s.grid[a.from].richness - 1]--;

    //std::cerr <<  s.info.trees_richness[player][s.grid[a.from].richness - 1] << std::endl;

	s.info.score[player] += (info + (s.grid[a.from].richness - 1) * 2);
    if (s.info.nutriments == 0)
        s.info.nutriments = 0;
	else
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
            new_s.info.trees_size[0][0]--;
            new_s.info.trees_richness[0][new_s.grid[a.to].richness - 1]--;
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
            this->info.trees_size[0][0]--;
            this->info.trees_richness[0][this->grid[a.to].richness - 1]--;
		}

		if (this->info.wait[0] and this->info.wait[1]) {
			pass_day(*this);
		}
	}
	this->info.player = !this->info.player;
}


static double  score_action(State& s, Action a) {
    double score = 0;
    int player = s.info.player;

    switch (a.type)
    {
        case Action_type::seed : {
            Cell& c = s.grid[a.to];
            Cell& from = s.grid[a.from];
            if (c.richness == 2 || c.richness == 1) {
                score = c.richness == 2 ? 0.8 : 0.8;
                if (s.info.trees_richness[player][2] >= 1 && s.info.trees_richness[player][1] < 3 && (c.richness == 2 || s.info.trees_richness[player][0] < 3) && from.size > 1) {
                    score += 0.1 * (6 - adjacent_trees(s, a.to, player)); // adjacent_trees returns the number of trees (you own) surrounding a cell
                }
            } else if (c.richness == 3) {
                score = s.info.trees_richness[player][2] < 4 ? 2 : 0;
                score -= (s.info.days < 8 ? 0.6 : 0.6 ) * adjacent_trees(s, a.to, player);
                score += from.size == 3 ? 0.1 : 0; // sow_size is the size of the tree in the cell sow
            }
            if (isDiagonal(a.from, a.to)) // is the cell id diagonal to the cell sow ?
                score += 0.2;
            if (s.info.days > 18)
                score -= 2;
            //std::cerr << "SON SCORE EST DE " << score << std::endl;
            break;
        }
        case Action_type::grow : {
            Cell& c = s.grid[a.from];
            if (c.richness == 1)
                score = 4;
            else if (c.richness == 2)
                score = 2.5;
            else if (c.richness == 3)
                score = 3;
            if (shadow_next_turn(s, s.info.days, a.from))
                score -= 1;
            score += 0.1 * (c.size + 1);
            break;
        }
        case Action_type::complete : {
            Cell& c = s.grid[a.from];
            if (c.richness == 1)
                score = 0;
            else
                score = c.richness;
            if (s.info.days > 21)
                score += 21;
            if (!shadow_next_turn(s, s.info.days, a.from))
                score -= 2;
            break;
        }
        default: {
            score = 1.5;
            break;
        }
    }
    return score;
}


Action  State::bestActionH() {
	int player = info.player;
    double best_score = 1.5;
    Action best_action;
    unsigned char prix;

    best_action = {Action_type::wait, 0, 0, 0};
	if (info.wait[player])
		return best_action;
    for (unsigned char i = 0; i < 37; i++) {
        if (!grid[i].empty and grid[i].player == player and !grid[i].sleep) {
            switch (grid[i].size)
            {
                case 0: {
                    prix = 1 + info.trees_size[player][1];
                    if (prix <= info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(a);
                        if (score > best_score) {
                            best_action = a;
                        }
                        best_score = score > best_score ? score : best_score;
                    }
                    break;
                }
                case 1: {
                    prix = 3 + info.trees_size[player][2];
                    if (prix <= info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(a);
                        if (score > best_score) {
                            best_action = a;
                        }
                        best_score = score > best_score ? score : best_score;
                    }
                    prix = info.trees_size[player][0];
                    if (prix > info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[0]; j++) {
                        int to = n[i].neigbours[j];
                        if (grid[to].empty and grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(a, player);
                            if (score > best_score) {
                                best_action = a;
                            }
                            best_score = score > best_score ? score : best_score;
                        }
                    }
                    break;
                }
                case 2: {
                    prix = 7 + info.trees_size[player][3];
                    if (prix <= info.sun[player]) {
                        Action a(Action_type::grow, i, 0, prix);
                        double score = score_grow(a);
                        if (score > best_score) {
                            best_action = a;
                        }
                        best_score = score > best_score ? score : best_score;
                    }                
                    prix = info.trees_size[player][0];
                    if (prix > info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[1]; j++) {
                        int to = n[i].neigbours[j];
                        if (grid[to].empty and grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(a, player);
                            if (score > best_score) {
                                best_action = a;
                            }
                            best_score = score > best_score ? score : best_score;
                        }
                    }
                    break;
                }
                case 3: {
                    if (info.sun[player] >= 4) {
                        Action a(Action_type::complete, i, 0, 4);
                        double score = score_complete(a);
                        if (score > best_score) {
                            best_action = a;
                        }
                        best_score = score > best_score ? score : best_score;
                    }
                    prix = info.trees_size[player][0];
                    if (prix > info.sun[player])
                        break;
                    for (int j = 0; j < n[i].size[2]; j++) {
                        int to = n[i].neigbours[j];
                        if (grid[to].empty and grid[to].richness != 0) {
                            Action a(Action_type::seed, i, to, prix);
                            double score = score_seed(a, player);
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

Action  best_actionH(State &s) {
    std::vector<Action> actions;
    s.generate_all_actions_no_score(actions);

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
    int i = 0;

    while (i < 20) {
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
        //simulation.do_action(simulation.bestActionH());
        simulation.do_action(simulation.random_action());
		//simulation.do_action({Action_type::wait, 0, 0, 0});
        i++;
    }
    if (state.info.player) {
        return 1 - simulation.eval_state();
    } else {
        return simulation.eval_state();
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

    std::cerr << i << std::endl;
    return root.best_action();
}


}


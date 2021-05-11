#pragma once

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <random>
#include <array>
#include <sstream>
#include <algorithm>
#include <functional>
#include <fstream>


int random_index(int lower, int higher);

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
std::ostream&   operator<<(std::ostream& f, Action_type a);

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
std::ostream&   operator<<(std::ostream& f, Action a);

struct State {
    Info                    info;
    std::array<Cell, 37>    grid;
    Action                  a;

	State() {}
	State(const Info& i, const std::array<Cell, 37>& g) : info(i), grid(g) {}
	void	generate_all_actions(std::vector<Action>& actions);
    int	    generate_all_actions2(std::array<Action, 5>& actions);
    void	generate_all_actions_no_score(std::vector<Action>& actions) const;

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

int     adjacent_trees(State& s, int index, int player);
bool    isDiagonal(int from, int to);
bool    inSecondLayer(int from, int to);
bool    shadow_next_turn(State& state, int day, int id);


void	pass_day(State &state);
bool	seed_action(State &s, Action a, int player, int info);
bool	complete_action(State &s, Action a, int player, int info);
bool	grow_action(State &s, Action a, int player, int info);
bool	wait_action(State &s, Action a, int player, int info);

Action  best_actionH(State &s);
Action  best_moove(State& s);
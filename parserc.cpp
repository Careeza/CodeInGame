#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <map>
#include <iostream>


struct neigbours {
	int size[3];
	int neigbours[36];
};

struct Action {

};

struct Cell {
	bool			player : 1; // player of ?
	bool			sleep : 1; // sleeping
	bool			empty : 1; // vide or not
	unsigned char	taille : 2; // 0 to 3
	unsigned char	richness : 2; // 0 to 3
	bool			info : 1; // info supplementaire bool dispo
}; // TAILLE TOTAL 1 Octet = taille char

struct	Info {
	bool			player : 1;
	bool			wait : 1; // player is waiting !!! if player is waiting no action avaible !!!
	unsigned char	days : 6; // 0 - 64 Les jours ne vont que a 24
	unsigned int	score : 12 ; // 0 - 4096
	unsigned int	enemy_score : 12; // 0 - 4096
	unsigned int	sun : 12 ; // 0 - 4096
	unsigned int	enemy_sun : 12 ; // 0 - 4096

	unsigned char	avaible : 8; //0 taille disponible pour info supp
}; // TAILLE TOTAL 8 Octet = taille long long int

enum struct Action_type {
	seed,
	grow,
	complete,
	wait,
	none,
};

struct Action {
    Action_type		type : 4; // 1 1 1 => 7
	unsigned char   from : 6; // index from
	unsigned char   to : 6; // index to for seed
    unsigned        size_left : 10; // size left for info
}; // TAILLE TOTAL 4 Octet = taille of int


class State {
    private:
		Cell	grid[37]; // taille 37 octet || Utiliser des arrays ??
		Info	info; // Taille 8 octet

		// std::vector<std::vector<std::vector<int>>>	a; Chercher une facon ingenieuse de le stocker
		//std::vector<Action> 	actions_avaible();
    public:
        State(const Cell g[37], const Info& i);
        
		//double              eval_state();
        //bool                final_state();
        
		//State               simulate(const Action& a);
        //void                move(const Action& a);
        //Action              random_action();
        //int                 state_player() {return player;}
};

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <random>
#include <array>

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
	bool			wait; // player is waiting !!! if player is waiting no action avaible !!!
	bool			enemy_wait; // player is waiting !!! if player is waiting no action avaible !!!
	unsigned char	days; // 0 - 64 Les jours ne vont que a 24
	unsigned short	score; // 0 - 4096
	unsigned short	enemy_score; // 0 - 4096
	unsigned short	sun; // 0 - 4096
	unsigned short	enemy_sun; // 0 - 4096

    std::array<int, 4>  trees_size;
    std::array<int, 4>  enemy_trees_size;

}; // TAILLE TOTAL 8 Octet = taille long long int

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
    Action_type		type : 4; // 1 1 1 => 7
	unsigned char   from : 6; // index from
	unsigned char   to : 6; // index to for seed
    unsigned char   cost : 6; 
    // Il reste 4 bytes

    Action(Action_type ty, unsigned char f, unsigned char t, unsigned char c) : type(ty), from(f), to(t), cost(c) {}
}; // TAILLE TOTAL 4 Octet = taille of int

std::ostream&   operator<<(std::ostream& f, Action a) {
    switch (a.type)
    {
    case Action_type::seed :
        f << a.type << " " << static_cast<int>(a.from) << " " << static_cast<int>(a.to) << std::endl;
        break;
    case Action_type::grow :
        f << a.type << " " << static_cast<int>(a.from) << std::endl;
        break;
    case Action_type::complete :
        f << a.type << " " << static_cast<int>(a.from) << std::endl;
        break;
    case Action_type::wait :
        f << a.type << std::endl;
        break;
    default:
        f << "Error in parser" << std::endl;
        break;
    }
    return f;
}

struct State {
    Info                    info;
    std::array<Cell, 37>    grid;
    Action                  a;
};

void    add_action(std::vector<Action>& actions, std::array<Cell, 37>& map, Info info, Action a) {
    int     prix = a.cost;
    bool    add = true;

    if (a.type == Action_type::seed) {
        if (!map[a.to].empty or map[a.to].richness == 0)
            add = false;
    }
    if (add and prix <= info.sun) {
        a.cost = prix;
        actions.push_back(a);
    }
}

void    generate_all_actions(std::vector<Action>& actions, std::array<Cell, 37>& map, Info info) {
    unsigned char prix;
    add_action(actions, map, info, {Action_type::wait, 0, 0, 0});
    for (unsigned char i = 0; i < 37; i++) {
        if (!map[i].empty and map[i].player == info.player) {
            switch (map[i].size)
            {
            case 0:
                prix = 1 + info.trees_size[1];
                add_action(actions, map, info, {Action_type::grow, i, 0, prix});
                break;
            case 1:
                prix = 3 + info.trees_size[2];
                add_action(actions, map, info, {Action_type::grow, i, 0, prix});
                for (int j = 0; j < n[i].size[0]; j++) {
                    prix = info.trees_size[0];
                    add_action(actions, map, info, {Action_type::seed, i, n[i].neigbours[j], prix});
                }
                break;
            case 2:
                prix = 7 + info.trees_size[3];
                add_action(actions, map, info, {Action_type::grow, i, 0, prix});
                for (int j = 0; j < n[i].size[1]; j++) {
                    prix = info.trees_size[0];;
                    add_action(actions, map, info, {Action_type::seed, i, n[i].neigbours[j], prix});
                }
                break;
            case 3:
                for (int j = 0; j < n[i].size[2]; j++) {
                    prix = info.trees_size[0];;
                    add_action(actions, map, info, {Action_type::seed, i, n[i].neigbours[j], prix});
                }
                add_action(actions, map, info, {Action_type::complete, i, 0, 4});
                break;
            default:
                std::cerr << "Error in parser" << std::endl;
                break;
            }
        }
    }
}

State   simulate_action(State& s, Action& a) {
    State   new_s(s);

    //new_s.info.player =
    if (!s.info.player) {

    }
}

int     main(int argc, char **argv) {
    std::array<Cell, 37>    map;
    Info                    info;

    int numberOfCells; // 37
    std::cin >> numberOfCells; std::cin.ignore();

    for (int i = 0; i < numberOfCells; i++) {
        int index, richness, neigh; 
        std::cin >> index >> richness >> neigh >> neigh >> neigh >> neigh >> neigh >> neigh; std::cin.ignore();
        map[index].richness = richness;
        map[index].empty = true;
    }
    while (1) {
        int skip;
        std::cin >> skip; std::cin.ignore();
        std::cin >> skip; std::cin.ignore();

        int sun;
        int score;

        std::cin >> sun >> score; std::cin.ignore();
        info.player = 0;
        info.sun = sun;
        info.score = score;

        int wait;
        std::cin >> sun >> score >> wait; std::cin.ignore();
        info.enemy_sun = sun;
        info.enemy_score = score;
        info.enemy_wait = wait;

        int numberOfTrees;
        std::cin >> numberOfTrees; std::cin.ignore();

        info.trees_size.fill(0);
        info.enemy_trees_size.fill(0);
        for (int i = 0; i < numberOfTrees; i++) {
            int cellIndex, size;
            bool isMine, isDormant;
            std::cin >> cellIndex >> size >> isMine >> isDormant; std::cin.ignore();
            map[cellIndex].size = size;
            map[cellIndex].player = !isMine;
            map[cellIndex].sleep = isDormant;
            map[cellIndex].empty = false;

            info.trees_size[size] += isMine;
        }

        int numberOfPossibleMoves;
        std::cin >> numberOfPossibleMoves; std::cin.ignore();
        std::string possibleMove;
        for (int i = 0; i < numberOfPossibleMoves; i++) {
            getline(std::cin, possibleMove);
            //std::cerr << "IA " << possibleMove << std::endl;
        }
        std::cout << possibleMove << std::endl;

        const std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();
        long long unsigned time;

        std::vector<Action> actions;
        for (int i = 0; i < 10000; i++) {
            actions.clear();
            generate_all_actions(actions, map, info);
        }

        const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        std::cerr << "TIME = " << time / 1000 << std::endl;
        // for (Action a : actions) {
        //     std::cerr << a;
        // }

        //return 0;
    }
}
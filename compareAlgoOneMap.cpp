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

    for (int p = 0; p < 2; p++) {
        for (int i = 0; i < 4 ; i++) {
            std::cerr << info.trees_size[p][i] << ", ";
        }
        std::cerr << std::endl;
    }
}



int     main(int argc, char **argv) {
    double  algo1 = 0;
    double  algo2 = 0;

    if (argc != 2)
        return 0;

    std::string route = "map/map" + std::to_string(atoi(argv[1])) + ".txt";

    std::ifstream in(route);
    std::streambuf *cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(in.rdbuf());

    std::cout << "Simulation sur la map : " << route << std::endl;

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
    info.trees_richness[0].fill(0);
    info.trees_richness[1].fill(0);
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

        info.trees_richness[!isMine][map[cellIndex].richness - 1]++;
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

    //std::cout << best_moove2(s) << std::endl;

    //std::cout << best_moove(s) << std::endl;

    int k = 0;

    while (!s.final_state()) {
        s.do_action(best_actionH(s));
        s.do_action(best_moove(s));
        k++;
    }

    std::cout << "SCORE FINAL best_moove2 = " << s.info.score[0] << std::endl;
    std::cout << "SCORE FINAL best_moove = " << s.info.score[1] << std::endl;

    if (s.info.score[0] > s.info.score[1]) {
        algo1 += 1;
    } else if (s.info.score[0] == s.info.score[1]) {
        algo1 += 0.5;
        algo2 += 0.5;
    } else {
        algo2 += 1;
    }

    std::cin.rdbuf(cinbuf);   //reset to standard input again
}
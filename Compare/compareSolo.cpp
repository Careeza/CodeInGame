#include "common.hpp"
#include "algo1.hpp"
#include "algo3.hpp"

# include <thread>

double  algo1_score = 0;

void  compare_algo(State start) {
    State s(start);
    State s2(start);
    double own_score = 0;

    while (!s.final_state()) {
        s.do_action(algo1::best_moove(s));
        s.do_action(algo3::best_moove(s));
    }
    if (s.info.score[0] > s.info.score[1]) {
        own_score += 1;
        algo1_score += 1;
    } else if (s.info.score[0] == s.info.score[1]) {
        own_score += 0.5;
        algo1_score += 0.5;
    }
    while (!s2.final_state()) {
        s2.do_action(algo3::best_moove(s2));
        s2.do_action(algo1::best_moove(s2));
    }
    if (s2.info.score[1] > s2.info.score[0]) {
        own_score += 1;
        algo1_score += 1;
    } else if (s2.info.score[0] == s2.info.score[1]) {
        own_score += 0.5;
        algo1_score += 0.5;
    }
    std::cout << "SCORE FINAL " << own_score << " vs " << 2 - own_score << std::endl;
}

State   parse_state(int i) {
    std::string route = "map/map" + std::to_string(i) + ".txt";

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

    std::cin.rdbuf(cinbuf);   //reset to standard input again

    return (State(info, map));
}

int     main(int argc, char **argv) {
    State s = parse_state(0);
    compare_algo(s);

    std::cout << "Fin de simulation :" << algo1_score << ", " << 400 - algo1_score << std::endl;
}
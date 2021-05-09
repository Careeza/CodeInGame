#define NB_JOURS 24

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <array>
#include <algorithm>
#include <numeric>                                                              
#include <tuple>


using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct Cell {
    int richness;
    std::array<int, 6>  neigh;
    std::array<int, 6>  threat;
    int                 average_threat;
    bool                has_tree;
};

struct Tree {
    int cellIndex;
    int size; // size of this tree: 0-3
    bool isMine; // 1 if this is your tree
    bool isDormant; // 1 if this tree is dormant
};

struct Player {
    int sun;
    int score;
    int wait;

};

struct Game {
    std::array<Cell, 37>    cells;

    int                 day;
    int                 nutrients;

    Player              p1;
    Player              p2;

    int                 numberOfTrees;
    std::vector<Tree>   trees;
    std::vector<Tree>   allies_trees;
    std::vector<Tree>   enemies_trees;
};

enum struct Action_type {
    plant,
    grow0,
    grow1,
    grow2,
    complete,
    NONE,
};

struct Action {
    Action_type type;
    int         from;
    int         to;
    Tree        *t;
};

Game game;

int max_size_tree() {
    int max = 0;
    for (Tree& t : game.allies_trees) {
        if (t.size > max) {
            max = t.size;
        }
    }
    return max;
}

bool has_tree_of_size(int size) {
    for (Tree& t : game.allies_trees) {
        if (t.size == size) {
            return true;
        }
    }
    return false;
}

bool    complete(Tree& t) {
    if (game.p1.sun >= 4) {
        std::cout << "COMPLETE " << t.cellIndex << std::endl;
        game.p1.sun -= 4;
        return true;
    }
    return false;
}

bool    grow(Tree& t) {
    int price = 0;

    if (t.size == 0)
        price = 1;
    if (t.size == 1)
        price = 3;
    else
        price = 7;

    for (Tree& t2 : game.allies_trees) {
        if (t2.size == t.size + 1) {
            price++;
        }
    }
    if (game.p1.sun >= price) {
        std::cout << "GROW " << t.cellIndex << std::endl;
        game.p1.sun -= price;
        return true;
    }
    return false;
}

bool    plant(int from, int to) {
    int price = 0;
    for (Tree& t2 : game.allies_trees) {
        if (t2.size == 0) {
            price++;
        }
    }
    if (game.p1.sun >= price) {
        std::cout << "SEED " << from << " " << to << std::endl;
        game.p1.sun -= price;
        return true;
    }
    return false;
}

bool  select_best_tree_to_plant(int& from, int& to) {
    int best_score = -100;
    bool has_plant = false;

    for (Tree &t : game.allies_trees) {
        if (t.isDormant or t.size == 0)
            continue;
        for (int index : game.cells[t.cellIndex].neigh) {
            if (index == -1 or game.cells[index].richness == 0 or game.cells[index].has_tree) {
                continue;
            }
            int score = game.cells[index].richness - game.cells[index].average_threat;
            if (score > best_score) {
                has_plant = true;
                from = t.cellIndex;
                to = index;
                best_score = score;
            }
        }
    }
    return has_plant;
}


Tree *select_best_tree_to_complete() {
    int best_richness = 0;
    Tree *best_tree = NULL;


    for (Tree& t : game.allies_trees) {
        if (t.isDormant)
            continue;
        if (t.size == 3) {
            if (game.cells[t.cellIndex].richness > best_richness) {
                best_tree = &t;
                best_richness = game.cells[t.cellIndex].richness;
            }
        }
    }
    return best_tree;

}

Tree *select_best_tree_to_grow() {
    int best_size = -10;
    int best_richness = 0;
    Tree *best_tree;


    for (Tree& t : game.allies_trees) {
        if (t.isDormant)
            continue;
        int richness = game.cells[t.cellIndex].richness;
        if (t.size != 3 and richness >= best_richness) {
            if (richness > best_richness) {
                best_tree = &t;
                best_size = t.size;
                best_richness = richness;
            } else {
                if (t.size == 0) {
                    best_tree = &t;
                    best_size = t.size;
                } 
                if (best_size != 0 and t.size > best_size) {
                    best_tree = &t;
                    best_size = t.size;
                }
            }
        }
    }
    return best_tree;
}

int     cost_action(Action_type action) {
    int price = 0;

    switch (action) {
        case Action_type::plant :
            for (Tree& t2 : game.allies_trees)
                if (t2.size == 0)
                    price++;
            break;
        case Action_type::grow0 :
            price = 1;
            for (Tree& t2 : game.allies_trees)
                if (t2.size == 1)
                    price++;
            break;
        case Action_type::grow1 :
            price = 3;
            for (Tree& t2 : game.allies_trees)
                if (t2.size == 2)
                    price++;
            break;
        case Action_type::grow2 :
            price = 7;
            for (Tree& t2 : game.allies_trees)
                if (t2.size == 3)
                    price++;
            break;
        case Action_type::complete :
            price = 4;
            break;
        default:
            break;
    }
    return price;
}

Action  select_best_action() {
    int from, to;
    Tree *t;
    int score = -42;
    Action action;

    action.type = Action_type::NONE;
    if ((t = select_best_tree_to_complete())) {
        int complete_score = game.cells[t->cellIndex].richness;
        std::cerr << "COMPLETE SCORE " << complete_score << std::endl;
        if (complete_score > score) {
            score = complete_score;
            action.type = Action_type::complete;
            action.t = t;
        }
    }

    if (game.day < 20 and game.allies_trees.size() < 10 and select_best_tree_to_plant(from, to)) {
        int plant_score = game.cells[to].richness;
        if (plant_score > score) {
            score = plant_score;
            action.type = Action_type::plant;
            action.from = from;
            action.to = to;
        }
    }

    if (game.day < 23 and (t = select_best_tree_to_grow())) {
        int grow_score = game.cells[t->cellIndex].richness;
        if (grow_score > score) {
            score = grow_score;
            switch (t->size) {
                case 0:
                    action.type = Action_type::grow0;
                    break;
                case 1:
                    action.type = Action_type::grow1;
                    break;
                case 2:
                    action.type = Action_type::grow2;
                    break;
                default:
                    action.type = Action_type::NONE;
                    break;
            }
            action.t = t;
        }
    }


    return action;
}

void    do_action(Action action) {
    int price = cost_action(action.type);

    if (price > game.p1.sun) {
        std::cerr << "TO EXPENSIVE" << std::endl;
        std::cout << "WAIT" << std::endl;
        return ;
    }
    switch (action.type) {
        case Action_type::plant :
            std::cout << "SEED " << action.from << " " << action.to << std::endl;
            break;
        case Action_type::grow0 :
        case Action_type::grow1 :
        case Action_type::grow2 :
            std::cout << "GROW " << action.t->cellIndex << std::endl;
            break;
        case Action_type::complete :
            std::cout << "COMPLETE " << action.t->cellIndex << std::endl;
            break;
        default :
            std::cerr << "NO ACTION" << std::endl;
            std::cout << "WAIT" << std::endl;
            break;
    }
}

int main()
{
    int numberOfCells; // 37
    cin >> numberOfCells; cin.ignore();

    for (int i = 0; i < numberOfCells; i++) {
        int index; // 0 is the center cell, the next cells spiral outwards

        Cell cell;
        cin >> index >> cell.richness;
        for (int i = 0; i < 6; i++) {
            cin >> cell.neigh[i];
        }
        cell.threat.fill(0);
        game.cells[index] = cell;
    }

    for (const Cell &cell : game.cells) {
        std::cerr << "{";
        for (int c : cell.neigh) {
            std::cerr << c << ", ";
        }
    std::cerr << "}" << std::endl;
        std::cerr << std::endl;
    }


    // game loop
    while (1) {
        std::cerr << "START" << std::endl;

        cin >> game.day; cin.ignore();
        cin >> game.nutrients; cin.ignore();

        cin >> game.p1.sun >> game.p1.score; cin.ignore();
        cin >> game.p2.sun >> game.p2.score >> game.p2.wait; cin.ignore();

        cin >> game.numberOfTrees; cin.ignore();

        std::map<int, Cell>::iterator   iter;
        // for(iter = game.cells.begin(); iter != game.cells.end(); iter++) {
        //     iter->second.threat.fill(0);
        //     iter->second.has_tree = false;
        // }
        game.trees = std::vector<Tree>(game.numberOfTrees);
        game.allies_trees.clear();
        game.enemies_trees.clear();
        for (int i = 0; i < game.numberOfTrees; i++) {
            cin >> game.trees[i].cellIndex >> game.trees[i].size >> game.trees[i].isMine >> game.trees[i].isDormant; cin.ignore();
            game.cells[game.trees[i].cellIndex].has_tree = true;
            if (game.trees[i].isMine)
                game.allies_trees.push_back(game.trees[i]);
            else                
                game.enemies_trees.push_back(game.trees[i]);
        }

        int numberOfPossibleMoves;
        cin >> numberOfPossibleMoves; cin.ignore();
        for (int i = 0; i < numberOfPossibleMoves; i++) {
            string possibleMove;
            getline(cin, possibleMove);
            //std::cerr << possibleMove << std::endl;
        }

        std::cerr << "END PARSE" << std::endl;

        //void threat_level();

        //bool has_done_action = false;

        // if (!has_done_action and game.day < 15) {
        //     int from, to;
        //     if (select_best_tree_to_plant(from, to)) {
        //         has_done_action = plant(from, to);
        //     }

        // }
        // if (!has_done_action and game.day < 24) {
        //     Tree *t = select_best_tree_to_grow();
        //     if (t and (game.day < 3 or t->size == 2)) {
        //         has_done_action = grow(*t);
        //     }
        // }
        // if (!has_done_action and game. day >= 20) {
        //     Tree *t = select_best_tree_to_complete();
        //     if (t) {
        //         has_done_action = complete(*t);
        //     }
        // }

        //std::cerr << "ACTION" << std::endl;
        Action action = select_best_action();
        do_action(action);
        //std::cerr << "HERE" << std::endl;
    }
}
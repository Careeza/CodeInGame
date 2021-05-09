#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <iostream>

std::mt19937 gen(std::random_device{}());

#define SQRT 1.41421356237

struct Action {
    int pos;

    Action(int p) : pos(p) {} 
};

struct Tree {
    double n;
    double w;
    Action              action;
    std::vector<Tree>   child;
    Tree                *father;

    Tree() : n(0), w(0), father(NULL), action(0) {}
    Tree(Action a, Tree* f) : n(0), w(0), action(a), father(f) {};
};

std::vector<std::vector<int>> map = {{0, 0, 0}, {0,0,0}, {0,0,0}};
std::array<std::array<int, 3>, 8> winPositions;
struct Grid {
    std::array<int, 9> content;

    std::array<int, 9> virtual_map;

    std::array<int, 9> simulation;
    Grid() {
        content.fill(0);

        winPositions[0] = { 0, 1, 2 };
        winPositions[1] = { 3, 4, 5 };
        winPositions[2] = { 6, 7, 8 };
        winPositions[3] = { 0, 3, 6 };
        winPositions[4] = { 1, 4, 7 };
        winPositions[5] = { 2, 5, 8 };
        winPositions[6] = { 0, 4, 8 };
        winPositions[7] = { 2, 4, 6 };
    };
};

std::ostream    &operator<<(std::ostream &f, std::array<int, 9>& g) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            f << g[i * 3 + j] << " ";
        }
        f << std::endl;
    }
    return f;
}

int random_index(int lower, int higher)
{
    return std::uniform_int_distribution<int>(lower, higher)(gen);
}

Tree    *selection(Grid& g, Tree *tree, int& player) {
    double best_score = -10;
    Tree *best_tree;

    if (tree->child.size() == 0)
        return tree;

    for (Tree &t : tree->child) {
        double score = t.w / t.n + SQRT * sqrt(log2(tree->n) / t.n);
        if (score > best_score) {
            best_score = score;
            best_tree = &t;
        }
    }
    g.simulation[tree->action.pos] = (player + 1) % 2 + 1;

    player++;

    return selection(g, best_tree, player);
}

int     gameWon(std::array<int, 9>& grid)
{
    for (const auto & line : winPositions) {
        if (grid[line[0]] == grid[line[1]]
                && grid[line[1]] == grid[line[2]]
                && grid[line[2]] != 0)
            return grid[line[0]];
    }
    return -1;
}

std::vector<Action> all_possible_mooves(std::array<int, 9> &grid) {
    std::vector<Action> actions;

    if (gameWon(grid) != -1)
        return actions;
    for (int i = 0; i < 9; i++) {
        if (grid[i] == 0)
            actions.emplace_back(i);
    }

    return actions;
}

Action  pick_random_moove(std::array<int, 9> &grid) {
    std::vector<Action> actions = all_possible_mooves(grid);

    if (actions.size() == 0) {
        return {-1};
    }
    return actions[random_index(0, actions.size() - 1)];
}

void    expension(Tree *t, Grid& grid) {
    std::vector<Action> actions = all_possible_mooves(grid.content);
    for (Action& action : actions) {
        t->child.emplace_back(action, t);
    }
}
int     simulation(Grid& grid, int player) {
    Action a = pick_random_moove(grid.simulation);

    if (a.pos == -1)
        return gameWon(grid.simulation);
    grid.simulation[a.pos] = (player + 1) % 2 + 1;

    int who_won = gameWon(grid.simulation);
    return who_won == -1 ? simulation(grid, player + 1) : who_won;
}

void    all_simulation(Tree *tree, Grid& g, int player) {
    for (Tree& t : tree->child) {
        t.n = 10;
        t.w = 0;

            g.simulation = g.virtual_map;
            g.simulation[t.action.pos] = player;
            int score = simulation(g, player);
            if (score == -1) {
                t.w += 0.5;
            } else if (score == player) {
                t.w += 1;
            } else {
                t.w += 0;
            }
    }
}

void    update(Tree *tree) {
    double new_exploration = 0;
    double win_rate = 0;

    for (Tree& t : tree->child) {
        win_rate += t.w;
        new_exploration += 1;
    }
    while (tree != NULL) {
        tree->n += new_exploration;
        tree->w += win_rate;
        tree = tree->father;
    }
}

Action  MCTS(Tree *tree, Grid& grid, int player) {
    int save_player = player;
    
    for (int i = 0; i < 1000; i++) {
        grid.virtual_map = grid.content;

        Tree *select_tree = selection(grid, tree, player);
        expension(select_tree, grid);
        all_simulation(select_tree, grid, player);
        update(select_tree);

        player = save_player;
    }
    
    double best_score = 0;
    Action best_action(0);
    for (Tree &t : tree->child) {
        //std::cout << t.n << std::endl;
        if (t.n > best_score) {
            best_action = t.action;
            best_score = t.n;
        }
    }
    return best_action;
}


int     main(void) {
    Grid    grid;
    Tree    t;
    
    int move;
    while (gameWon(grid.content) == -1) {
        std::cin >> move;

        grid.content[move] = 1;

        Action a = MCTS(&t, grid, 2);
        t.child.clear();

        //std::cout << "IA PLAYED : " << a.pos << std::endl;

        grid.content[a.pos] = 2;

        std::cout << grid.content << std::endl;
    }

    return (0);
}

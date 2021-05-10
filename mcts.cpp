#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <iostream>

std::mt19937 gen(std::random_device{}());

int random_index(int lower, int higher) {
    return std::uniform_int_distribution<int>(lower, higher)(gen);
}


int winPositions[8][3] = {{ 0, 1, 2 },
                        { 3, 4, 5 },
                        { 6, 7, 8 },
                        { 0, 3, 6 },
                        { 1, 4, 7 },
                        { 2, 5, 8 },
                        { 0, 4, 8 },
                        { 2, 4, 6 }};

struct Action {
    Action(int p, int pl) : pos(p), player(pl) {};
    int pos;
    int player;
};

class State {
    friend State;
    friend std::ostream& operator<<(std::ostream& f, const State& s);
    private:
        int                 grid[9];
        int                 player;

    public:
        State(int g[9], int p);
        double              eval_state();
        bool                final_state();
        std::vector<Action> actions_avaible();
        State               simulate(const Action& a);
        void                move(const Action& a);
        Action              random_action();
        int                 state_player() {return player;}
        void                print(std::string space = "") {
            std::cout << space << "Player to play = " << player + 1 << std::endl;
            for (int i = 0; i < 3; i++) {
                std::cout << space;
                for (int j = 0; j < 3; j++) {
                    switch (grid[i * 3 + j]) {
                        case 0:
                            std::cout << "O";
                            break;
                        case 1:
                            std::cout << "X";
                            break;
                        default:
                            std::cout << ".";
                    }
                    std::cout << " ";
                }
                std::cout << std::endl;
            }
        }
};

State::State(int g[9], int p) : player(p) {
    for (int i = 0; i < 9; i++) {
        grid[i] = g[i];
    }
}

double  State::eval_state() {
    for (const auto & line : winPositions) {
        if (grid[line[0]] == grid[line[1]] && grid[line[1]] == grid[line[2]] && grid[line[2]] != -1) {
            if (grid[line[0]] == player) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    return 0.5;
}

bool    State::final_state() {
    double state = eval_state();
    
    //std::cout << "state" << state << std::endl;

    //std::cout << *this << std::endl;

    if (state < 0.45 || state > 0.55) {
        //std::cout << *this << std::endl;
        return true;
    }
    for (const auto & c : grid) {
        if (c == -1)
            return false;
    }
    return true;
}

std::vector<Action> State::actions_avaible() {
    std::vector<Action> a;

    if (final_state())
        return a;
    for (int i = 0; i < 9; i++) {
        if (grid[i] == -1)
            a.emplace_back(i, player);
    }
    return a;
}

Action  State::random_action() {
    std::vector<Action> actions = actions_avaible();

    int index = random_index(0, actions.size() - 1);

    actions[index].player = player;

    return actions[index];
}

State   State::simulate(const Action& a) {
    State new_state = *this;

    new_state.grid[a.pos] = a.player;

    new_state.player = (player + 1) % 2;

    return new_state;
}

void    State::move(const Action& a) {
    grid[a.pos] = a.player;
}


std::ostream&   operator<<(std::ostream& f, const State& s) {
    f << "Player to play = " << s.player + 1 << std::endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            switch (s.grid[i * 3 + j]) {
                case 0:
                    f << "O";
                    break;
                case 1:
                    f << "X";
                    break;
                default:
                    f << ".";
            }
            f << " ";
        }
        f << std::endl;
    }
    return f;
}

class Tree {
    friend std::ostream& operator<<(std::ostream& f, Tree& s);
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

        Action  best_action() const;

        void    print_all_state() {
            std::cout << "INIT STATE" << std::endl;
            std::cout << state << std::endl;

            std::cout << "visit = " << n << ", win = " << w << std::endl;

            for (int i = 0; i < child.size(); i++) {
                std::cout << "CHILD No " << i + 1 << std::endl;
                std::cout << child[i].state << std::endl;
            }
        }

        void    print(std::string space = "") {

            std::cout << space << "STATE" << std::endl;
            state.print(space);

            std::cout << space << "n = " << n << std::endl;
            std::cout << space << "w = " << w << std::endl;


            int i = 1;
            space = space + "    ";
            for (Tree& t : child) {
                std::cout << space << "CHILD no " << i << std::endl;
                double UCT_SCORE = (1 - t.score()) + sqrt(2) * sqrt(log2(n) / t.n);
                std::cout << space << "UCT SCORE = " << UCT_SCORE << std::endl;
                t.print(space);
                std::cout << std::endl;
                i++;
            }
            //std::cout << std::endl
        }
};


std::ostream& operator<<(std::ostream& f, Tree& s) {
    f << "STATE" << std::endl;

    f << s.state;

    f << "n = " << s.n << std::endl;
    f << "w = " << s.w << std::endl;


    int i = 1;
    for (Tree& t : s.child) {
        f << "CHILD no " << i << std::endl;
        double UCT_SCORE = (1 - t.score()) + sqrt(2) * sqrt(log2(s.n) / t.n);
        f << "UCT SCORE = " << UCT_SCORE << std::endl;
        f << t << std::endl;
        i++;
    }

    return f;
}


Tree::Tree(const State& s, Tree *f) : state(s), father(f), w(0), n(0) {}

Tree    *Tree::select_tree() {
    double best_score = -10;
    Tree *best_tree;


    if (child.size() == 0)
        return this;
    
    for (Tree& t : child) {
        if (t.n == 0) {
            return t.select_tree();
        }
        //std::cout << "score = " << 2 * sqrt(log2(n) / t.n) << std::endl;
        double score = (1 - t.score()) + 2 * sqrt(log2(n) / t.n);
        if (score > best_score) {
            best_score = score;
            best_tree = &t;
        }
    }
    return best_tree->select_tree();
}

void    Tree::expand() {
    actions = state.actions_avaible();

    if (child.size() != 0) {
        std::cerr << "PROB IN THE ALGO" << std::endl;
        exit (0);
    }

    for (Action a : actions) {
        child.emplace_back(state.simulate(a), this);
    }
}

double  Tree::simulate() {
    State simulation(state);

    while (1) {
        if (simulation.final_state()) {
            // std::cout << "END SIMULATION" << std::endl;
            //std::cout << simulation << std::endl;
            return simulation.eval_state();
        }
        Action a = simulation.random_action();

        simulation.move(a);
    }
    return 0;
}

Action  Tree::best_action() const {
    int     best_score = -10;
    int     index = 0;
    int     best_index = 0;
    
    for (const Tree& t : child) {
        int score = t.nb_exploration();

        //std::cerr << "score = " << score << std::endl;

        if (score > best_score) {
            best_score = score;
            best_index = index;
        }
        index++;
    }

    //std::cerr << actions.size() << std::endl;

    return actions[best_index];
}

void    Tree::back_track(double score) {
    n += 1;
    w += score;

    // std::cout << state << std::endl;
    // std::cout <<  "SCORE = " << score << std::endl;

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
    } while (time < 100000);
    std::cout << i << std::endl;
    return root.best_action();
}

void    test() {
    int grid1[9] = { 1, 1, 1, 
                    1, 0, 0,
                    -1, -1, 0};
    int grid2[9] = { 0, 0, 1, 
                    1, 0, 0,
                    -1, -1, 1};
    int grid3[9] = { 0, 0, 1, 
                    1, 0, 1,
                    -1, -1, 1};
    int grid4[9] = { 0, 0, 0, 
                    1, 0, 1,
                    1, 1, 0};
    int grid5[9] = { 1, 1, 1, 
                    0, 1, 0,
                    0, 0, 1};
    int grid6[9] = { 0, 1, 1, 
                    0, 0, 1,
                    1, 1, 0};
    
    State s1_1(grid1, 0);
    State s1_2(grid1, 1);

    State s2_1(grid2, 0);
    State s2_2(grid2, 1);

    State s3_1(grid3, 0);
    State s3_2(grid3, 1);

    State s4_1(grid4, 0);
    State s4_2(grid4, 1);

    State s5_1(grid5, 0);
    State s5_2(grid5, 1);

    State s6_1(grid6, 0);
    State s6_2(grid6, 1);

    std::cout << s1_1.eval_state() << " - " << s1_2.eval_state() << std::endl;
    std::cout << s2_1.eval_state() << " - " << s2_2.eval_state() << std::endl;
    std::cout << s3_1.eval_state() << " - " << s3_2.eval_state() << std::endl;
    std::cout << s4_1.eval_state() << " - " << s4_2.eval_state() << std::endl;
    std::cout << s5_1.eval_state() << " - " << s5_2.eval_state() << std::endl;
    std::cout << s6_1.eval_state() << " - " << s6_2.eval_state() << std::endl;

    exit (0);
}


int     main(void) {
    // int grid[9] = { 0, 0, 1, 
    //                 1, 0, 0,
    //                 -1, -1, 1
    //             };
    //int grid[9] = {0, 0, 1, 0, -1, 1, -1, -1, -1};
    //int grid[9] = {0, 0, 1, -1, 1, -1, 0, -1, -1};

    //int grid[9] = {0, -1, 1, -1, -1, -1, -1, -1, -1};
    int grid[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    State init(grid, 0);

    //test();

    std::cout << init << std::endl;
    while (!init.final_state()) {
        // int i, j;
        // std::cin >> i >> j;
        // Action p_action((i - 1) * 3 + (j - 1), 0);

        // init = init.simulate(p_action);

        // std::cout << init << std::endl;

        // if (init.final_state())
        //     break;

        Action a = best_moove(init);
        init = init.simulate(a);

        
        std::cout << init << std::endl;


    }

    // std::cout << init << std::endl;
    // Action a = best_moove(init);
    // init = init.simulate(a);
    // std::cout << init << std::endl;
    return 0;
}
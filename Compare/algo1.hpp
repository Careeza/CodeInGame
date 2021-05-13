#include "common.hpp"

namespace algo1 {
    double	eval_state(State& s);
    double  score_seed(State& s, Action& a, int player);
    double  score_grow(State& s, Action& a);
    double  score_complete(State& s, Action& a, int player);
    Action  bestActionH(State& s);
    void	generate_all_actions(State& s, std::vector<Action>& actions);
    void	generate_all_actions_no_score(State& s, std::vector<Action>& actions);
    Action	random_action(State& s);
    Action  best_actionH(State &s);

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

    Action  best_moove(State& s);
}


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
            if (inSecondLayer(a.from, a.to)) // is the cell id in the second layer of neighbors of cell sow ?
                score += 0.1;
            if (isDiagonal(a.from, a.to)) // is the cell id diagonal to the cell sow ?
                score += 0.1;
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
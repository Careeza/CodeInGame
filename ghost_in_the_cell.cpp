#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

enum struct Strategy {
    expension,
    defense,
    growth,
};

struct Factory {
    std::vector<int>        distance;
    std::vector<Factory *>  neighbours;

    int                     player;
    int                     nb_unit;

    int                     predicted_unit;
    int                     enemy_unit_otw;

    int                     neutral_neighbours;
    int                     enemy_neighbours;
    int                     ally_neighbours;

    int                     active;
    int                     production;
    int                     id;

    int                     value;

    int                     threat_level;
};

struct Info {
    std::vector<Factory>    all_factories;

    Strategy                strategy;

    int                     nb_of_factory;
    int                     nb_enemy_factory;

    int                     nb_of_units;
    int                     nb_enemy_units;

    int                     current_growth;
    int                     enemy_groth;
};

void    add_factory(Factory& f, Factory& f2, int distance) {
    int id1 = f.id;
    int id2 = f2.id;

    f.distance[id2] = distance;
    f2.distance[id1] = distance;
    f.neighbours[id2] = &f2;
    f2.neighbours[id1] = &f;
}

void    reset_info(Info &info) {
    for (Factory& f : info.all_factories) {
        f.enemy_unit_otw = 0;
        f.predicted_unit = 0;
    }
    info.nb_of_units = 0;
    info.nb_enemy_units = 0;
    info.current_growth = 0;
    info.nb_of_factory = 0;
    info.nb_enemy_factory = 0;
}

void    update_factory_info(Factory& factory) {
    factory.neutral_neighbours = 0;
    factory.enemy_neighbours = 0;
    factory.ally_neighbours = 0;
    for (Factory* f : factory.neighbours) {
        if (f->player == 0) {
            factory.neutral_neighbours++;
        } else if (f->player == 1) {
            factory.ally_neighbours++;
        } else {
            factory.enemy_neighbours++;
        }
    }
    factory.threat_level = factory.predicted_unit - factory.enemy_unit_otw;

    if (factory.player == 1) {
        factory.value = factory.production;
        factory.value -= factory.active;
        factory.value += factory.enemy_neighbours;
    } else if (factory.player == -1) {
        factory.value = factory.production;
        factory.value -= factory.active;
        factory.value += factory.ally_neighbours;
        factory.value *= 5;
        factory.value -= factory.nb_unit;
    } else {
        factory.value = factory.production;
        factory.value -= factory.active;
        factory.value += factory.ally_neighbours;
        factory.value *= 5;
        factory.value -= factory.nb_unit;
    }
}
void    update_factory(int arg1, int arg2, int arg3, int arg4, int arg5, int factory_id, Info& info) {
    info.all_factories[factory_id].player = arg1;
    info.all_factories[factory_id].nb_unit = arg2;
    info.all_factories[factory_id].production = arg3;
    info.all_factories[factory_id].active = arg4;

    if (arg1 == 1) {
        info.nb_of_units += arg2;
        info.current_growth += arg3;
    } else if (arg1 == -1) {
        info.nb_enemy_units += arg2;
        info.enemy_groth += arg3;
    } else {
        //
    }
}

void    update_troops(int arg1, int arg2, int arg3, int arg4, int arg5, Info& info) {
    Factory& target = info.all_factories[arg3];

    if (arg1 == target.player) {
        target.predicted_unit += arg4;
    } else {
        target.enemy_unit_otw += arg4;
    }

    if (arg1 == 1) {
        info.nb_of_units += arg4;
    } else {
        info.nb_enemy_units += arg4;
    }
}

void    update_info(Info &info) {
    int     enemy_factory_threaten = 0;
    int     ally_factory_threaten = 0;

    for (Factory& f : info.all_factories) {
        update_factory_info(f);
        if (f.player == 1) {
            info.nb_of_factory++;
            if (f.threat_level > 0) {
                ally_factory_threaten++;
            }
        } else if (f.player == -1) {
            info.nb_enemy_factory++;
            if (f.threat_level > 0) {
                enemy_factory_threaten++;
            }
        }
    }

    if (enemy_factory_threaten < ally_factory_threaten) {
        info.strategy = Strategy::defense;
    } else {
        if (info.nb_of_factory < info.all_factories.size() / 3) {
            info.strategy = Strategy::expension;
        } else {
            info.strategy = Strategy::growth;
        }
    }
}

void    growth_strategy(Info& info) {
    for (Factory& f : info.all_factories) {
        if (f.player == 1) {
            if (f.threat_level <= 0) {
                if (f.production < 3 and f.nb_unit > 10) {
                    cout << "INC " << f.id << "; ";
                    f.nb_unit -= 10;
                }
                for (Factory* f2 : f.neighbours) {
                    if (f2->player == 1) {
                        if (f2->threat_level > 0) {
                            cout << "MOVE" << f.id << " " << f2->id << " " << f.threat_level << "; ";
                            f.nb_unit -= f.threat_level;
                            f2->threat_level -= f.threat_level;
                        }
                    }
                }
            }
        }
    }
}

void    defense_strategy(Info& info) {
    for (Factory& f : info.all_factories) {
        if (f.player == 1 and f.threat_level <= 0) {
            Factory *facory_to_defend = NULL;
            int factory_to_defend_value = -1;
            for (Factory* f2 : f.neighbours) {
                if (f2->player == 1 and f2->threat_level > 0) {
                    if (f2->value > factory_to_defend_value) {
                        factory_to_defend_value = f2->value;
                        facory_to_defend = f2;
                    }
                }
            }
            if (facory_to_defend != NULL) {
                int troops_to_send = facory_to_defend->threat_level > -f.threat_level ? -f.threat_level : facory_to_defend->threat_level;
                cout << "MOVE" << f.id << " " << facory_to_defend->id << " " << troops_to_send << "; ";
                f.threat_level += troops_to_send;
                facory_to_defend->threat_level -= troops_to_send;
            }
        }
    }
}

void    expension_strategy(Info& info) {
    for (Factory& f : info.all_factories) {
        if (f.player == 1 and f.threat_level <= 0) {
            int factory_to_attack_value = -1;
            Factory *facory_to_attack = NULL;
            do {
                for (Factory* f2 : f.neighbours) {
                    if (f2->player != 1) {
                        if (f2->value > factory_to_attack_value) {
                            factory_to_attack_value = f2->value;
                            facory_to_attack = f2;
                        }
                    }
                }
                if (facory_to_attack != NULL) {
                    int troops_to_send = facory_to_attack->threat_level * 2 > -f.threat_level ? -f.threat_level : facory_to_attack->threat_level * 2;
                    cout << "MOVE" << f.id << " " << facory_to_attack->id << " " << troops_to_send << "; ";
                    f.threat_level += troops_to_send;
                }
            } while (facory_to_attack != NULL and f.threat_level < 0);
        }
    }
}

void    move(Info &info, int turn) {
    static int nb_bombs = 2;

    if (nb_bombs > 0) {
        Factory *target = NULL;
        for (Factory &f : info.all_factories) { //peut facilemnent etre ameliorer
            if (f.player == -1 and target == NULL) {
                target = &f;
            }
        }
        Factory *sender;
        for (Factory &f : info.all_factories) {
            if (f.player == 1 and sender == NULL) {
                sender = &f;
            }
        }
        if (sender != NULL and target != NULL) {
            nb_bombs--;
            cout << "BOMB " << sender->id << " " << target->id << "; ";
        }
    }
    if (info.strategy == Strategy::defense) {
        defense_strategy(info);
    } else if (info.strategy == Strategy::expension) {
        expension_strategy(info);
    } else {
        growth_strategy(info);
    }
}

int main()
{
    int factoryCount; // the number of factories
    cin >> factoryCount; cin.ignore();

    std::vector<Factory>    factories(factoryCount);

    for (size_t i = 0; i < factories.size(); i++) {
        factories[i].id = i;
        factories[i].distance = vector<int>(factoryCount, 0);
        factories[i].neighbours = vector<Factory *>(factoryCount, NULL);
    }

    int linkCount; // the number of links between factories
    cin >> linkCount; cin.ignore();
    for (int i = 0; i < linkCount; i++) {
        int factory1;
        int factory2;
        int distance;
        cin >> factory1 >> factory2 >> distance; cin.ignore();
        add_factory(factories[factory1], factories[factory2], distance);
    }

    Info    info;

    // game loop
    while (1) {
        int factory_id = 0;
        int entityCount; // the number of entities (e.g. factories and troops)
        cin >> entityCount; cin.ignore();
        reset_info(info);
        for (int i = 0; i < entityCount; i++) {
            int entityId;
            string entityType;
            int arg1;
            int arg2;
            int arg3;
            int arg4;
            int arg5;
            cin >> entityId >> entityType >> arg1 >> arg2 >> arg3 >> arg4 >> arg5; cin.ignore();

            if (entityType == "FACTORY") {
                update_factory(arg1, arg2, arg3, arg4, arg5, factory_id, info);
                factory_id++;
            } else if (entityType == "TROOP") {
                update_troops(arg1, arg2, arg3, arg4, arg5, info);
            } else {

            }
        }
        update_info(info);

        move(info);

        cout << "WAIT" << endl;
    }
}
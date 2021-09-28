#pragma GCC optimize("O3")
#pragma GCC optimize("inline")
#pragma GCC optimize("omit-frame-pointer")
#pragma GCC optimize("unroll-loops")
#include "MyStrategy.hpp"
#include <exception>
#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <cassert>
#include <thread>
#include <algorithm>
#include <numeric>
using std::vector;
using std::pair, std::make_pair;
using std::min, std::max, std::abs;
#define ULL unsigned long long
#define LL long long
#define Grid vector<vector<pair<char, int>>>
#define VPCI vector<pair<char, int>>
#define VI vector<int>
#define VVI vector<vector<int>>
#define VVVI vector<vector<vector<int>>>
#define VVVVI vector<vector<vector<vector<int>>>>

////////// Globals //////////
Action globResult;
const int GlobResourceNeeded = 1000;
const int mapSize = 80;
int globTick = 0;
int globMyId = -1;
int globMyResourceLeft;
double globBuilderShare;
double globRangedShare;
double globMeleeShare;
bool globHaveRangedBase;
int globMyBuilderPopulation;
int globMyMeleePopulation;
int globMyRangedPopulation;
int globMyTotalPopulation;
int globMyCurrentPossiblePopulation;
int globMyPlannedPossiblePopulation;
int globMyTotalHouses; // Including planned or under construction
int globMyCompletedHouses;
int globMyRangedBases;
double globDangerLevel;
int globMyRetreats;
vector<bool> globActionDone;
bool globNewBuilt = true;

///////// Houses ////////////
struct house {
    EntityType type;
    pair<int, int> pos;
    pair<int, int> n1;
    pair<int, int> n2;
    pair<int, int> n3;
};
const vector<house> globPossibleBuildings =
{ {HOUSE, {0, 0}, {2, 3}, {1, 3}, {0, 3}},
{HOUSE, {3, 0}, {5, 3}, {4, 3}, {3, 3}}, {HOUSE, {0, 4}, {3, 6}, {3, 5}, {3, 4}},
{HOUSE, {6, 0}, {8, 3}, {7, 3}, {6, 3}}, {HOUSE, {0, 7}, {3, 9}, {3, 8}, {3, 7}},
{HOUSE, {9, 0}, {11, 3}, {10, 3}, {9, 3}}, {HOUSE, {0, 10}, {3, 12}, {3, 11}, {3, 10}},
{HOUSE, {12, 0}, {14, 3}, {13, 3}, {12, 3}}, {HOUSE, {0, 13}, {3, 15}, {3, 14}, {3, 13}},
{HOUSE, {15, 0}, {17, 3}, {16, 3}, {15, 3}}, {HOUSE, {0, 16}, {3, 18}, {3, 17}, {3, 16}},
{HOUSE, {11, 4}, {13, 7}, {11, 7}, {13, 3}}, {HOUSE, {4, 11}, {7, 13}, {7, 11}, {3, 13}},
{HOUSE, {14, 4}, {16, 7}, {15, 7}, {16, 3}}, {HOUSE, {4, 14}, {7, 16}, {7, 15}, {3, 16}},
{HOUSE, {17, 4}, {19, 7}, {18, 7}, {19, 3}}, {HOUSE, {4, 17}, {7, 19}, {7, 18}, {3, 19}},
{ RANGED_BASE, {4, 12}}, { RANGED_BASE, {12, 4}},
{ RANGED_BASE, {8, 12}}, { RANGED_BASE, {12, 8}},
{ RANGED_BASE, {5, 12}}, { RANGED_BASE, {12, 5}},
{ RANGED_BASE, {9, 12}}, { RANGED_BASE, {12, 9}},
{ RANGED_BASE, {12, 12}},
{ RANGED_BASE, {6, 12}}, { RANGED_BASE, {12, 6}},
{ RANGED_BASE, {10, 12}}, { RANGED_BASE, {12, 10}},
{ RANGED_BASE, {7, 12}}, { RANGED_BASE, {12, 7}},
{ RANGED_BASE, {11, 12}}, { RANGED_BASE, {12, 11}},
{HOUSE, {18, 0}, {20, 3}, {19, 3}, {18, 3}}, {HOUSE, {0, 19}, {3, 21}, {3, 20}, {3, 19}},
{HOUSE, {12, 8}, {14, 11}, {12, 11}, {14, 7}}, {HOUSE, {8, 12}, {11, 14}, {11, 12}, {7, 14}},
{HOUSE, {15, 8}, {17, 11}, {15, 11}, {17, 7}}, {HOUSE, {8, 15}, {11, 17}, {11, 15}, {7, 17}},

{ RANGED_BASE, {4, 13}}, { RANGED_BASE, {13, 4}},
{ RANGED_BASE, {8, 13}}, { RANGED_BASE, {13, 8}},
{ RANGED_BASE, {5, 13}}, { RANGED_BASE, {13, 5}},
{ RANGED_BASE, {9, 13}}, { RANGED_BASE, {13, 9}},
{ RANGED_BASE, {13, 13}},
{ RANGED_BASE, {6, 13}}, { RANGED_BASE, {13, 6}},
{ RANGED_BASE, {10, 13}}, { RANGED_BASE, {13, 10}},
{ RANGED_BASE, {7, 13}}, { RANGED_BASE, {13, 7}},
{ RANGED_BASE, {11, 13}}, { RANGED_BASE, {13, 11}},

{ RANGED_BASE, {4, 14}}, { RANGED_BASE, {14, 4}},
{ RANGED_BASE, {8, 14}}, { RANGED_BASE, {14, 8}},
{ RANGED_BASE, {5, 14}}, { RANGED_BASE, {14, 5}},
{ RANGED_BASE, {9, 14}}, { RANGED_BASE, {14, 9}},
{ RANGED_BASE, {14, 14}},
{ RANGED_BASE, {6, 14}}, { RANGED_BASE, {14, 6}},
{ RANGED_BASE, {10, 14}}, { RANGED_BASE, {14, 10}},
{ RANGED_BASE, {7, 14}}, { RANGED_BASE, {14, 7}},
{ RANGED_BASE, {11, 14}}, { RANGED_BASE, {14, 11}},

{ RANGED_BASE, {4, 15}}, { RANGED_BASE, {15, 4}},
{ RANGED_BASE, {8, 15}}, { RANGED_BASE, {15, 8}},
{ RANGED_BASE, {5, 15}}, { RANGED_BASE, {15, 5}},
{ RANGED_BASE, {9, 15}}, { RANGED_BASE, {15, 9}},
{ RANGED_BASE, {15, 15}},
{ RANGED_BASE, {6, 15}}, { RANGED_BASE, {15, 6}},
{ RANGED_BASE, {10, 15}}, { RANGED_BASE, {15, 10}},
{ RANGED_BASE, {7, 15}}, { RANGED_BASE, {15, 7}},
{ RANGED_BASE, {11, 15}}, { RANGED_BASE, {15, 11}},

{ RANGED_BASE, {4, 16}}, { RANGED_BASE, {16, 4}},
{ RANGED_BASE, {8, 16}}, { RANGED_BASE, {16, 8}},
{ RANGED_BASE, {5, 16}}, { RANGED_BASE, {16, 5}},
{ RANGED_BASE, {9, 16}}, { RANGED_BASE, {16, 9}},
{ RANGED_BASE, {16, 16}},
{ RANGED_BASE, {6, 16}}, { RANGED_BASE, {16, 6}},
{ RANGED_BASE, {10, 16}}, { RANGED_BASE, {16, 10}},
{ RANGED_BASE, {7, 16}}, { RANGED_BASE, {16, 7}},
{ RANGED_BASE, {11, 16}}, { RANGED_BASE, {16, 11}},

{ RANGED_BASE, {4, 17}}, { RANGED_BASE, {17, 4}},
{ RANGED_BASE, {8, 17}}, { RANGED_BASE, {17, 8}},
{ RANGED_BASE, {5, 17}}, { RANGED_BASE, {17, 5}},
{ RANGED_BASE, {9, 17}}, { RANGED_BASE, {17, 9}},
{ RANGED_BASE, {17, 17}},
{ RANGED_BASE, {6, 17}}, { RANGED_BASE, {17, 6}},
{ RANGED_BASE, {10, 17}}, { RANGED_BASE, {17, 10}},
{ RANGED_BASE, {7, 17}}, { RANGED_BASE, {17, 7}},
{ RANGED_BASE, {11, 17}}, { RANGED_BASE, {17, 11}},

{ RANGED_BASE, {0, 18}}, { RANGED_BASE, {18, 0}},
{ RANGED_BASE, {4, 18}}, { RANGED_BASE, {18, 4}},
{ RANGED_BASE, {8, 18}}, { RANGED_BASE, {18, 8}},
{ RANGED_BASE, {1, 18}}, { RANGED_BASE, {18, 1}},
{ RANGED_BASE, {5, 18}}, { RANGED_BASE, {18, 5}},
{ RANGED_BASE, {9, 18}}, { RANGED_BASE, {18, 9}},
{ RANGED_BASE, {18, 18}},
{ RANGED_BASE, {2, 18}}, { RANGED_BASE, {18, 2}},
{ RANGED_BASE, {6, 18}}, { RANGED_BASE, {18, 6}},
{ RANGED_BASE, {10, 18}}, { RANGED_BASE, {18, 10}},
{ RANGED_BASE, {3, 18}}, { RANGED_BASE, {18, 3}},
{ RANGED_BASE, {7, 18}}, { RANGED_BASE, {18, 7}},
{ RANGED_BASE, {11, 18}}, { RANGED_BASE, {18, 11}},

{ RANGED_BASE, {0, 19}}, { RANGED_BASE, {19, 0}},
{ RANGED_BASE, {4, 19}}, { RANGED_BASE, {19, 4}},
{ RANGED_BASE, {8, 19}}, { RANGED_BASE, {19, 8}},
{ RANGED_BASE, {1, 19}}, { RANGED_BASE, {19, 1}},
{ RANGED_BASE, {5, 19}}, { RANGED_BASE, {19, 5}},
{ RANGED_BASE, {9, 19}}, { RANGED_BASE, {19, 9}},
{ RANGED_BASE, {19, 19}},
{ RANGED_BASE, {2, 19}}, { RANGED_BASE, {19, 2}},
{ RANGED_BASE, {6, 19}}, { RANGED_BASE, {19, 6}},
{ RANGED_BASE, {10, 19}}, { RANGED_BASE, {19, 10}},
{ RANGED_BASE, {3, 19}}, { RANGED_BASE, {19, 3}},
{ RANGED_BASE, {7, 19}}, { RANGED_BASE, {19, 7}},
{ RANGED_BASE, {11, 19}}, { RANGED_BASE, {19, 11}},

{ RANGED_BASE, {0, 20} }, { RANGED_BASE, {20, 0} },
{ RANGED_BASE, {4, 20} }, { RANGED_BASE, {20, 4} },
{ RANGED_BASE, {8, 20} }, { RANGED_BASE, {20, 8} },
{ RANGED_BASE, {1, 20} }, { RANGED_BASE, {20, 1} },
{ RANGED_BASE, {5, 20} }, { RANGED_BASE, {20, 5} },
{ RANGED_BASE, {9, 20} }, { RANGED_BASE, {20, 9} },
{ RANGED_BASE, {20, 20} },
{ RANGED_BASE, {2, 20} }, { RANGED_BASE, {20, 2} },
{ RANGED_BASE, {6, 20} }, { RANGED_BASE, {20, 6} },
{ RANGED_BASE, {10, 20} }, { RANGED_BASE, {20, 10} },
{ RANGED_BASE, {3, 20} }, { RANGED_BASE, {20, 3} },
{ RANGED_BASE, {7, 20} }, { RANGED_BASE, {20, 7} },
{ RANGED_BASE, {11, 20} }, { RANGED_BASE, {20, 11} },

{ RANGED_BASE, {0, 21} }, { RANGED_BASE, {21, 0} },
{ RANGED_BASE, {4, 21} }, { RANGED_BASE, {21, 4} },
{ RANGED_BASE, {8, 21} }, { RANGED_BASE, {21, 8} },
{ RANGED_BASE, {1, 21} }, { RANGED_BASE, {21, 1} },
{ RANGED_BASE, {5, 21} }, { RANGED_BASE, {21, 5} },
{ RANGED_BASE, {9, 21} }, { RANGED_BASE, {21, 9} },
{ RANGED_BASE, {21, 21} },
{ RANGED_BASE, {2, 21} }, { RANGED_BASE, {21, 2} },
{ RANGED_BASE, {6, 21} }, { RANGED_BASE, {21, 6} },
{ RANGED_BASE, {10, 21} }, { RANGED_BASE, {21, 10} },
{ RANGED_BASE, {3, 21} }, { RANGED_BASE, {21, 3} },
{ RANGED_BASE, {7, 21} }, { RANGED_BASE, {21, 7} },
{ RANGED_BASE, {11, 21} }, { RANGED_BASE, {21, 11} },
{ HOUSE, {21, 0}, {23, 3}, {22, 3}, {21, 3} }, { HOUSE, {0, 22}, {3, 24}, {3, 23}, {3, 22} },
{ HOUSE, {24, 0}, {26, 3}, {25, 3}, {24, 3} }, { HOUSE, {0, 25}, {3, 27}, {3, 26}, {3, 25} },
{ HOUSE, {20, 4 }, {22, 7 }, {21, 7 }, {22, 3 } }, { HOUSE, {4, 20}, {7, 22}, {7, 21}, {3, 22} },
{ HOUSE, { 18, 8}, { 20, 11}, { 18, 11}, {20, 7} }, { HOUSE, {8, 18}, {11, 20}, {11, 18}, {7, 20} }
};
vector<bool> globBuiltBuildings;
vector<bool> globIncompleteBuildings;
vector<bool> globFreeBuildings;
vector<bool> globTreatedBuildings;
vector<pair<double, pair<int, int>>> globBuildingFields;
vector<vector<pair<int, int>>> globBuildBuildingPosition;
vector<vector<pair<int, int>>> globPlannedPos;
vector<vector<EntityType>> globBuildBuildingType;

//////// Global Grids //////
vector<pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
vector<pair<int, int>> diag_neighbors = { {1, 1}, {-1, 1}, {1, -1}, {-1, -1} };
std::map<pair<int, int>, int> globCoorToGameId;
std::map<pair<int, int>, int> globCoorToEntry;
Grid globGrid(mapSize, vector<pair<char, int>>(mapSize, { '#', -1 }));
vector<vector<int>> close_opponent_fighters;
vector<vector<int>> outside_opponent_range;
vector<vector<int>> close_own_non_builders;
vector<vector<int>> globInOpponentsRange;
vector<vector<bool>> globRetreats;
vector<vector<bool>> globStays;
vector<vector<bool>> globAttackClosest;
vector<pair<int, int>> globBuilderPositions;
vector<vector<double>> globDangerFields(mapSize, vector<double>(mapSize, 0));
vector<vector<int>> globBestNeighbor;
vector<pair<double, pair<int, int>>> globBestFields;
VVVVI globDistances(mapSize, VVVI(mapSize, VVI(mapSize, VI(mapSize, -1)))); // indices are {from_y, from_x, to_y, to_x}
std::vector<std::vector<int>> globDistancesChanged(mapSize, vector<int>(mapSize, true)); // have the distances from this point maybe changed?
vector<vector<bool>> globWillOccupy;
vector<vector<int>> globClosestResourceDistance;
vector<vector<int>> globClosestFreeResourceDistance;
vector<vector<bool>> globCurrentVisible;
////////////////////////////

/////////Parameters/////////
const double HOUSE_NUMBER_MUL = 3.5;
const double LABOR_SUM_MULTIMPLIER = 1;
const int HOUSES_BEFORE_BASE = 8;
const double SUB_FOR_BUILT = 30;
const double HOUSE_BUILDER_VALUES[3] = { 1000.0, 500.0, 150.0 };
const double ALL_ADJACENT_BUILDER = 350;
const double BEHIND_MUL = 0.4;
const double OWN_NEGATIVE_DANGER = 0.0;
const double BUILDER_DANGER = 0.25;
const double RESOURCE_MUL = 0.4;
const double DANGER_MUL = 1.5;
const double DIAG_MUL = 0.4;
const int ADD_DIST_SEARCH = 40;
const bool PRINT_GRID = false;
const bool PRINT_VISIBLE = false;
const bool PRINT_RETREATS = false;
const bool PRINT_OPPONENT_RANGE = false;
const bool PRINT_UNIT_DIFF = false;
const int OWN_CLOSE = 3;
const int MAX_OWN_UNITS = 200;
////////////////////////////

//#define ARI_DEBUG
#ifdef _DEBUG
#if _MSC_VER == 1927
bool globDebug = true;
#else
bool globDebug = false;
#endif
#else
bool globDebug = false;
#endif

// Small helpers
namespace {
    void sleep(int ms) {
        assert(globDebug);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    inline bool is_valid(int y, int x) {
        return y >= 0 && y < mapSize&& x >= 0 && x < mapSize;
    }

    void set_unitShares() { // Sets what shares of unit types I should have
        if (globHaveRangedBase) {
            globBuilderShare = 1.0 - (double)globTick / 1000. - globDangerLevel / 400.;
        }
        else {
            globBuilderShare = 1.0;
        }
        globRangedShare = 1.0 - globBuilderShare;
        globMeleeShare = 0.0;
    }

    void set_allSeen() {
        globCurrentVisible.assign(mapSize, vector<bool>(mapSize, true));
    }

    bool is_own_fighter(const Entity& entity) {
        auto typ = entity.entityType;
        return (typ == RANGED_UNIT || typ == MELEE_UNIT) && *entity.playerId == globMyId;
    }

    bool is_own_unit(const Entity& entity) {
        auto typ = entity.entityType;
        return (typ == BUILDER_UNIT || typ == RANGED_UNIT || typ == MELEE_UNIT) && *entity.playerId == globMyId;
    }

    bool is_opponent_fighter(const Entity& entity) {
        auto typ = entity.entityType;
        return (typ == RANGED_UNIT || typ == MELEE_UNIT) && *entity.playerId != globMyId;
    }

    bool is_opponent_unit(const Entity& entity) {
        auto typ = entity.entityType;
        return (typ == BUILDER_UNIT || typ == RANGED_UNIT || typ == MELEE_UNIT) && *entity.playerId != globMyId;
    }

    bool angle_cmp(pair<int, int> p1, pair<int, int> p2) {
        return (double)p1.second / (double)p1.first < (double)p2.second / (double)p2.first;
    }

    bool second_angle_cmp(pair<pair<int, int>, int> p1, pair<pair<int, int>, int> p2) {
        return (double)p1.first.second / (double)p1.first.first < (double)p2.first.second / (double)p2.first.first;
    }

    double builderDistMul() {
        return 4000. / (globTick + 5ll);
    }

    double resourceVal() {
        return RESOURCE_MUL * (globTick + 200.0) / 1000.0;
    }

    double houseBuilderValue(int builder) {
        return HOUSE_BUILDER_VALUES[builder] * 100.0 / (globTick - 19.9);
    }

    void reset_visible(Grid& grid, const vector<vector<bool>>& visible) {
        for (int g_y = 0; g_y < mapSize; g_y++) {
            for (int g_x = 0; g_x < mapSize; g_x++) {
                if (visible.at(g_y).at(g_x) || grid.at(g_y).at(g_x).first != '#') {
                    grid.at(g_y).at(g_x) = make_pair('.', -1);
                }
            }
        }
    }

    bool resource_removed(const Grid& grid, const Grid& oldGrid) {
        for (int g_y = 0; g_y < mapSize; g_y++) {
            for (int g_x = 0; g_x < mapSize; g_x++) {
                if (oldGrid.at(g_y).at(g_x).first == '#' && grid.at(g_y).at(g_x).first != '#') {
                    return true;
                }
            }
        }
        return false;
    }
}


// Big helpers
namespace {
    void set_allAdjacentBuildingFields(const PlayerView& playerView, int b_id) {
        auto [ent_y, ent_x] = globPossibleBuildings.at(b_id).pos;
        EntityType entityType = globPossibleBuildings.at(b_id).type;
        int ent_sz = playerView.entityProperties.at(entityType).size;
        for (int y_o = 0; y_o < ent_sz; y_o++) {
            int a_y = ent_y + y_o, a_x = ent_x - 1;
            if (is_valid(a_y, a_x) && (globGrid.at(a_y).at(a_x).first == '.' || globGrid.at(a_y).at(a_x).first == 'B')) {
                globBuildingFields.push_back({ ALL_ADJACENT_BUILDER * BEHIND_MUL, {a_y, a_x} });
                globBuildBuildingPosition.at(a_y).at(a_x) = globPossibleBuildings.at(b_id).pos;
                globBuildBuildingType.at(a_y).at(a_x) = globPossibleBuildings.at(b_id).type;
            }
            a_x = ent_x + ent_sz;
            if (is_valid(a_y, a_x) && (globGrid.at(a_y).at(a_x).first == '.' || globGrid.at(a_y).at(a_x).first == 'B')) {
                globBuildingFields.push_back({ ALL_ADJACENT_BUILDER, {a_y, a_x} });
                globBuildBuildingPosition.at(a_y).at(a_x) = globPossibleBuildings.at(b_id).pos;
                globBuildBuildingType.at(a_y).at(a_x) = globPossibleBuildings.at(b_id).type;
            }
        }
        for (int x_o = 0; x_o < ent_sz; x_o++) {
            int a_y = ent_y - 1, a_x = ent_x + x_o;
            if (is_valid(a_y, a_x) && (globGrid.at(a_y).at(a_x).first == '.' || globGrid.at(a_y).at(a_x).first == 'B')) {
                globBuildingFields.push_back({ ALL_ADJACENT_BUILDER * BEHIND_MUL, {a_y, a_x} });
                globBuildBuildingPosition.at(a_y).at(a_x) = globPossibleBuildings.at(b_id).pos;
                globBuildBuildingType.at(a_y).at(a_x) = globPossibleBuildings.at(b_id).type;
            }
            a_y = ent_y + ent_sz, a_x = ent_x + x_o;
            if (is_valid(a_y, a_x) && (globGrid.at(a_y).at(a_x).first == '.' || globGrid.at(a_y).at(a_x).first == 'B')) {
                globBuildingFields.push_back({ ALL_ADJACENT_BUILDER, {a_y, a_x} });
                globBuildBuildingPosition.at(a_y).at(a_x) = globPossibleBuildings.at(b_id).pos;
                globBuildBuildingType.at(a_y).at(a_x) = globPossibleBuildings.at(b_id).type;
            }
        }
    }

    void set_globUnitCounts(const PlayerView& playerView) {
        globMyBuilderPopulation = 0;
        globMyMeleePopulation = 0;
        globMyRangedPopulation = 0;
        globMyTotalPopulation = 0;
        globMyCurrentPossiblePopulation = 0;
        globMyTotalHouses = 0;
        globMyCompletedHouses = 0;
        globMyRangedBases = 0;
        for (size_t i = 0; i < playerView.entities.size(); i++) {
            const Entity& entity = playerView.entities[i];
            if (entity.playerId == nullptr || *entity.playerId != globMyId) {
                continue;
            }
            const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
            if (entity.entityType == BUILDER_UNIT)
                globMyBuilderPopulation += properties.populationUse;
            else if (entity.entityType == MELEE_UNIT)
                globMyMeleePopulation += properties.populationUse;
            else if (entity.entityType == RANGED_UNIT)
                globMyRangedPopulation += properties.populationUse;
            else if (entity.entityType == HOUSE) {
                globMyTotalHouses++;
                if (entity.health == properties.maxHealth) {
                    globMyCompletedHouses++;
                }
            }
            else if (entity.entityType == RANGED_BASE && entity.active)
                globMyRangedBases++;
            globMyTotalPopulation += properties.populationUse;
            if (entity.active)
                globMyCurrentPossiblePopulation += properties.populationProvide;
        }
    }

    void adapt_globBuilderDistancesChanged(bool fieldChanged) {
        if (fieldChanged) {
            globDistancesChanged.assign(mapSize, vector<int>(mapSize, true));
        }
    }

    void set_globBuilderDistances() {
        for (int b_y = 0; b_y < mapSize; b_y++) {
            for (int b_x = 0; b_x < mapSize; b_x++) {
                if (globGrid.at(b_y).at(b_x).first == 'B' && globGrid.at(b_y).at(b_x).second == globMyId) { // own builder
                    if (globDistancesChanged.at(b_y).at(b_x)) {
                        globDistancesChanged.at(b_y).at(b_x) = false;
                        vector<vector<int>>& dists = globDistances.at(b_y).at(b_x);
                        dists.assign(mapSize, vector<int>(mapSize, -1));
                        int maxDist = min(90, globClosestResourceDistance.at(b_y).at(b_x) + ADD_DIST_SEARCH);
                        std::queue<int> q; //{{y, x}, dist}
                        q.push((b_y << 20) + (b_x << 10) + 0);
                        while (!q.empty()) {
                            int qf = q.front(); q.pop();
                            int p_y = qf >> 20, p_x = (qf >> 10) % (1 << 10);
                            int dist = qf % (1 << 10);
                            if (dists[p_y][p_x] == -1) {
                                dists[p_y][p_x] = dist;
                                for (int n = 0; n < 4; n++) {
                                    int n_y = p_y + directions.at(n).first, n_x = p_x + directions.at(n).second;
                                    if (is_valid(n_y, n_x) && dists[n_y][n_x] == -1) {
                                        auto [typ, owner] = globGrid[n_y][n_x];
                                        if ((typ == '.' || (owner == globMyId && (typ == 'B' || typ == 'R' || typ == 'M'))) && dist < maxDist) {
                                            q.emplace((n_y << 20) + (n_x << 10) + (dist + 1));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //PRE: L and R initialized with (-1)s
    //POST: Sets L and R to match in O(n^3)
    void get_minAssignment(const vector<vector<double>>& c, vector<int>& L, vector<int> R) {
        //std::cerr << "In g_minAss\n";
        if (c.size() == 0) {
            return;
        }
        int n = c.size(), m = c[0].size();
        assert(n <= m);
        vector<double> v(m), dist(m); // v : potential
        vector<int> index(m), prev(m);
        std::iota(index.begin(), index.end(), 0);
        auto residue = [&](int i, int j) { return c[i][j] - v[j]; };
        //std::cerr << "Bef for g_minAss\n";
        for (int f = 0; f < n; ++f) {
            for (int j = 0; j < m; ++j) {
                dist[j] = residue(f, j); prev[j] = f;
            }
            double w; int j, l;
            for (int s = 0, t = 0;;) {
                if (s == t) {
                    l = s; w = dist[index[t++]];
                    for (int k = t; k < m; ++k) {
                        j = index[k]; double h = dist[j];
                        if (h <= w) {
                            if (h < w) { t = s; w = h; }
                            index[k] = index[t]; index[t++] = j;
                        }
                    }
                    for (int k = s; k < t; ++k) {
                        j = index[k];
                        if (R[j] < 0) goto aug;
                    }
                }
                int q = index[s++], i = R[q];
                for (int k = t; k < m; ++k) {
                    j = index[k];
                    double h = residue(i, j) - residue(i, q) + w;
                    if (h < dist[j]) {
                        dist[j] = h; prev[j] = i;
                        if (h == w) {
                            if (R[j] < 0) goto aug;
                            index[k] = index[t]; index[t++] = j;
                        }
                    }
                }
            }
        aug:
            for (int k = 0; k < l; ++k)
                v[index[k]] += dist[index[k]] - w;
            int i;
            do {
                R[j] = i = prev[j];
                std::swap(j, L[i]);
            } while (i != f);
        }
        double ret = 0;
        for (int i = 0; i < n; ++i) {
            ret += c[i][L[i]]; // ( i , L[ i ]) is a solution
        }
        return;
    }
}

// Grids, output
namespace {
    void set_retreat(int p_y, int p_x) {
        assert(!globAttackClosest.at(p_y).at(p_x));
        assert(globRetreats.at(p_y).at(p_x) + globStays.at(p_y).at(p_x) == 1);
        char typ = globGrid.at(p_y).at(p_x).first;
        assert(typ == 'M' || typ == 'R' || typ == 'B');
        assert(globCoorToGameId.count({ p_y, p_x }));
        if (!(typ == 'M' || typ == 'R' || typ == 'B')) {
            return;
        }
        int n_y = p_y, n_x = p_x;
        if (globStays.at(p_y).at(p_x)) {
            // Don't change coordinates
        }
        else if (p_y > p_x && globGrid.at(p_y - 1ll).at(p_x).first == '.') {
            n_y--;
        }
        else if (p_x > 0 && globGrid.at(p_y).at(p_x - 1ll).first == '.') {
            n_x--;
        }
        else if (p_y > 0 && globGrid.at(p_y - 1ll).at(p_x).first == '.') {
            n_y--;
        }
        else { // No retreat towards base possible, let routing algorithm find retreat
            n_y = 0;
            n_x = 0;
        }
        if (globStays.at(p_y).at(p_x)) {
            std::shared_ptr<MoveAction> moveAction = std::shared_ptr<MoveAction>(new MoveAction(Vec2Int(n_x, n_y), true, true));
            globResult.entityActions[globCoorToGameId.at({ p_y, p_x })] = EntityAction(
                moveAction,
                nullptr,
                std::shared_ptr<AttackAction>(new AttackAction(
                    nullptr, std::shared_ptr<AutoAttack>(new AutoAttack(5, vector<EntityType>())))),
                nullptr);
        }
        else {
            std::shared_ptr<MoveAction> moveAction = std::shared_ptr<MoveAction>(new MoveAction(Vec2Int(n_x, n_y), true, false));
            globResult.entityActions[globCoorToGameId.at({ p_y, p_x })] = EntityAction(
                moveAction,
                nullptr,
                nullptr,
                nullptr);
        }
        globActionDone.at(globCoorToEntry.at({ p_y, p_x })) = true;
        globMyRetreats++;
    }

    void set_currentVisible(const PlayerView& playerView) {
        globCurrentVisible.assign(mapSize, vector<bool>(mapSize, false));
        for (int i = 0; i < playerView.entities.size(); i++) {
            const Entity& entity = playerView.entities.at(i);
            if (entity.playerId == nullptr || *entity.playerId != globMyId) {
                continue;
            }
            EntityType typ = entity.entityType;
            int ent_sz = playerView.entityProperties.at(typ).size;
            int ent_sight = playerView.entityProperties.at(typ).sightRange;
            auto [ent_x, ent_y] = entity.position;
            for (int s_y = ent_y; s_y < ent_y + ent_sz; s_y++) {
                for (int s_x = ent_x; s_x < ent_x + ent_sz; s_x++) {
                    for (int f_y = max(0, s_y - ent_sight); f_y < min(mapSize, s_y + ent_sight + 1); f_y++) {
                        int x_r = ent_sight - abs(f_y - s_y);
                        for (int f_x = max(0, s_x - x_r); f_x < min(mapSize, s_x + x_r + 1); f_x++) {
                            globCurrentVisible.at(f_y).at(f_x) = true;
                        }
                    }
                }
            }
        }
    }

    void set_grids(Grid& grid, Grid& oldGrid, const PlayerView& playerView) {
        oldGrid = grid;
        reset_visible(grid, globCurrentVisible);
        globCoorToGameId.clear();
        globCoorToEntry.clear();
        globHaveRangedBase = false;
        for (size_t i = 0; i < playerView.entities.size(); i++) {
            const Entity& entity = playerView.entities[i];
            const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
            int ent_y = entity.position.y, ent_x = entity.position.x;
            int ent_sz = properties.size;
            int owner = entity.playerId == nullptr ? 0 : *entity.playerId;
            for (int p_y = ent_y; p_y < ent_y + ent_sz; p_y++) {
                for (int p_x = ent_x; p_x < ent_x + ent_sz; p_x++) {
                    globGrid.at(p_y).at(p_x).second = owner;
                    char& type = grid.at(p_y).at(p_x).first;
                    const int& eT = entity.entityType;
                    if (eT == RESOURCE)
                        type = '#';
                    else if (eT == BUILDER_UNIT)
                        type = 'B';
                    else if (eT == MELEE_UNIT)
                        type = 'M';
                    else if (eT == RANGED_UNIT)
                        type = 'R';
                    else if (eT == RANGED_BASE && owner == globMyId) {
                        type = '@';
                        globHaveRangedBase = true;
                    }
                    else if (eT == MELEE_BASE || eT == RANGED_BASE || eT == BUILDER_BASE)
                        type = '@';
                    else if (eT == HOUSE)
                        type = 'H';
                    else if (eT == TURRET)
                        type = 'T';
                    globCoorToGameId.insert({ {p_y, p_x}, entity.id });
                    globCoorToEntry.insert({ {p_y, p_x}, (int)i });
                }
            }
        }
        close_opponent_fighters.assign(mapSize, vector<int>(mapSize, 0));
        outside_opponent_range.assign(mapSize, vector<int>(mapSize, 9));
        close_own_non_builders.assign(mapSize, vector<int>(mapSize, 0));
        globInOpponentsRange.assign(mapSize, vector<int>(mapSize, 0));
        for (int at_y = 0; at_y < mapSize; at_y++) {
            for (int at_x = 0; at_x < mapSize; at_x++) {
                const char type = globGrid.at(at_y).at(at_x).first;
                const int owner = globGrid.at(at_y).at(at_x).second;
                if (owner <= 0 || owner == globMyId || !(type == 'M' || type == 'R' || type == 'T')) {
                    continue;
                }
                if (type == 'M') {
                    for (int c_y = std::max(0, at_y - 9); c_y <= std::min(mapSize - 1, at_y + 9); c_y++) {
                        for (int c_x = std::max(0, at_x - (9 - std::abs(at_y - c_y))); c_x <= std::min(mapSize - 1, at_x + (9 - std::abs(at_y - c_y))); c_x++) {
                            int dist = std::abs(c_y - at_y) + std::abs(c_x - at_x);
                            outside_opponent_range.at(c_y).at(c_x) = std::min(outside_opponent_range.at(c_y).at(c_x), dist - 1);
                            close_opponent_fighters.at(c_y).at(c_x)++;
                            if (dist <= 1) {
                                globInOpponentsRange.at(c_y).at(c_x)++;
                            }
                        }
                    }
                }
                else if (type == 'R' || (type == 'T' && playerView.entities.at(globCoorToEntry.at({at_y, at_x})).active)) {
                    assert(type == 'R' || type == 'T');
                    for (int c_y = std::max(0, at_y - 9); c_y <= std::min(mapSize - 1, at_y + 9); c_y++) {
                        for (int c_x = std::max(0, at_x - (9 - std::abs(at_y - c_y))); c_x <= std::min(mapSize - 1, at_x + (9 - std::abs(at_y - c_y))); c_x++) {
                            int dist = std::abs(c_y - at_y) + std::abs(c_x - at_x);
                            outside_opponent_range.at(c_y).at(c_x) = std::min(outside_opponent_range.at(c_y).at(c_x), std::max(0, dist - 5));
                            close_opponent_fighters.at(c_y).at(c_x)++;
                            if (dist <= 5) {
                                globInOpponentsRange.at(c_y).at(c_x)++;
                            }
                        }
                    }
                }
            }
        }

        for (int at_y = 0; at_y < mapSize; at_y++) {
            for (int at_x = 0; at_x < mapSize; at_x++) {
                const char type = globGrid.at(at_y).at(at_x).first;
                const int owner = globGrid.at(at_y).at(at_x).second;
                if (owner != globMyId || type == 'B' || type == '@' || type == 'H' || type == 'T') {
                    continue;
                }
                for (int c_y = std::max(0, at_y - OWN_CLOSE); c_y <= std::min(mapSize - 1, at_y + OWN_CLOSE); c_y++) {
                    for (int c_x = std::max(0, at_x - (OWN_CLOSE - std::abs(at_y - c_y))); c_x <= std::min(mapSize - 1, at_x + (OWN_CLOSE - std::abs(at_y - c_y))); c_x++) {
                        close_own_non_builders.at(c_y).at(c_x)++;
                    }
                }
            }
        }
        globRetreats.assign(mapSize, vector<bool>(mapSize, false));
        globStays.assign(mapSize, vector<bool>(mapSize, false));
        globAttackClosest.assign(mapSize, vector<bool>(mapSize, false));
        for (int at_y = 0; at_y < mapSize; at_y++) {
            for (int at_x = 0; at_x < mapSize; at_x++) {
                const char type = globGrid.at(at_y).at(at_x).first;
                const int owner = globGrid.at(at_y).at(at_x).second;
                if (owner != globMyId) {
                    continue;
                }
                bool retreat = (outside_opponent_range.at(at_y).at(at_x) <= 2 && outside_opponent_range.at(at_y).at(at_x) > 0);
                if (close_own_non_builders.at(at_y).at(at_x) > close_opponent_fighters.at(at_y).at(at_x)) {
                    retreat = false;
                }
                if (retreat) {
                    if (outside_opponent_range.at(at_y).at(at_x) == 1 || type == 'B')
                        globRetreats.at(at_y).at(at_x) = true;
                    else
                        globStays.at(at_y).at(at_x) = true;
                }
                else if (outside_opponent_range.at(at_y).at(at_x) <= 2 && type != 'B') {
                    globAttackClosest.at(at_y).at(at_x) = true;
                }
            }
        }
        bool attackChanged = true;
        while (attackChanged) {
            attackChanged = false;
            for (int at_y = 0; at_y < mapSize; at_y++) {
                for (int at_x = 0; at_x < mapSize; at_x++) {
                    if (globAttackClosest.at(at_y).at(at_x)) {
                        for (int c_y = std::max(0, at_y - OWN_CLOSE); c_y <= std::min(mapSize - 1, at_y + OWN_CLOSE); c_y++) {
                            for (int c_x = std::max(0, at_x - (OWN_CLOSE - std::abs(at_y - c_y))); c_x <= std::min(mapSize - 1, at_x + (OWN_CLOSE - std::abs(at_y - c_y))); c_x++) {
                                if (globRetreats.at(c_y).at(c_x) || globStays.at(c_y).at(c_x)) {
                                    globRetreats.at(c_y).at(c_x) = false;
                                    globStays.at(c_y).at(c_x) = false;
                                    globAttackClosest.at(c_y).at(c_x) = true;
                                    attackChanged = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void set_globDangerFields() {
        for (int d_y = 0; d_y < mapSize; d_y++) {
            for (int d_x = 0; d_x < mapSize; d_x++) {
                globDangerFields.at(d_y).at(d_x) *= 0.6;
                if (globClosestResourceDistance.at(d_y).at(d_x) == 0) {
                    for (int c_y = std::max(0, d_y - 8); c_y <= std::min(mapSize - 1, d_y + 8); c_y++) {
                        for (int c_x = std::max(0, d_x - (8 - std::abs(d_y - c_y))); c_x <= std::min(mapSize - 1, d_x + (8 - std::abs(d_y - c_y))); c_x++) {
                            const char& typ = globGrid[c_y][c_x].first;
                            const char& owner = globGrid[c_y][c_x].second;
                            if (typ == 'M' || typ == 'R') {
                                if (owner != globMyId) {
                                    globDangerFields.at(d_y).at(d_x) += 10.0 - ((double)std::abs(c_y - d_y) + std::abs(c_x - d_x));
                                }
                            }
                            else if (typ == 'T') {
                                if (owner != globMyId && std::abs(c_y - d_y) + std::abs(c_x - d_x) <= 6) {
                                    globDangerFields.at(d_y).at(d_x) += 10.0 - ((double)std::abs(c_y - d_y) + std::abs(c_x - d_x));
                                }
                            }
                            else if (typ == 'B') {
                                if (owner != globMyId) {
                                    globDangerFields.at(d_y).at(d_x) += BUILDER_DANGER * (10.0 - ((double)std::abs(c_y - d_y) + std::abs(c_x - d_x)));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void set_globClosestResourceDistance() {
        globClosestResourceDistance.assign(mapSize, vector<int>(mapSize, -1));
        std::queue<pair<pair<int, int>, int>> q;
        for (int f_y = 0; f_y < mapSize; f_y++) {
            for (int f_x = 0; f_x < mapSize; f_x++) {
                char f_typ = globGrid.at(f_y).at(f_x).first;
                if (f_typ != '.' && f_typ != 'B') {
                    continue;
                }
                bool has_resource_nei = false;
                for (int fn = 0; fn < 4; fn++) {
                    int fn_y = f_y + directions.at(fn).first, fn_x = f_x + directions.at(fn).second;
                    if (is_valid(fn_y, fn_x)) {
                        char fn_typ = globGrid.at(fn_y).at(fn_x).first;
                        if (fn_typ == '#') {
                            has_resource_nei = true;
                            break;
                        }
                    }
                }
                if (has_resource_nei) {
                    q.push({ {f_y, f_x}, 0 });
                }
            }
        }
        while (!q.empty()) {
            auto [pos, dist] = q.front(); q.pop();
            auto [p_y, p_x] = pos;
            if (globClosestResourceDistance.at(p_y).at(p_x) == -1) {
                globClosestResourceDistance.at(p_y).at(p_x) = dist;
                for (int n = 0; n < 4; n++) {
                    int n_y = p_y + directions.at(n).first, n_x = p_x + directions.at(n).second;
                    if (is_valid(n_y, n_x) && globClosestResourceDistance.at(n_y).at(n_x) == -1) {
                        auto [typ, owner] = globGrid.at(n_y).at(n_x);
                        if (typ == '.' || (owner == globMyId && (typ == 'B' || typ == 'R' || typ == 'M'))) {
                            q.push({ {n_y, n_x}, dist + 1 });
                        }
                    }
                }
            }
        }
    }

    void set_globClosestFreeResourceDistance() {
        globClosestFreeResourceDistance.assign(mapSize, vector<int>(mapSize, -1));
        std::queue<pair<pair<int, int>, int>> q;
        for (int f_y = 0; f_y < mapSize; f_y++) {
            for (int f_x = 0; f_x < mapSize; f_x++) {
                char f_typ = globGrid.at(f_y).at(f_x).first;
                if (f_typ != '.' && f_typ != 'B') {
                    continue;
                }
                if (globWillOccupy.at(f_y).at(f_x)) { // Builder already (going) there
                    continue;
                }
                bool has_resource_nei = false;
                for (int fn = 0; fn < 4; fn++) {
                    int fn_y = f_y + directions.at(fn).first, fn_x = f_x + directions.at(fn).second;
                    if (is_valid(fn_y, fn_x)) {
                        char fn_typ = globGrid.at(fn_y).at(fn_x).first;
                        if (fn_typ == '#') {
                            has_resource_nei = true;
                            break;
                        }
                    }
                }
                if (has_resource_nei) {
                    q.push({ {f_y, f_x}, 0 });
                }
            }
        }
        while (!q.empty()) {
            auto [pos, dist] = q.front(); q.pop();
            auto [p_y, p_x] = pos;
            if (globClosestFreeResourceDistance.at(p_y).at(p_x) == -1) {
                globClosestFreeResourceDistance.at(p_y).at(p_x) = dist;
                for (int n = 0; n < 4; n++) {
                    int n_y = p_y + directions.at(n).first, n_x = p_x + directions.at(n).second;
                    if (is_valid(n_y, n_x) && globClosestFreeResourceDistance.at(n_y).at(n_x) == -1) {
                        auto [typ, owner] = globGrid.at(n_y).at(n_x);
                        if (typ == '.' || (owner == globMyId && (typ == 'B' || typ == 'R' || typ == 'M'))) {
                            q.push({ {n_y, n_x}, dist + 1 });
                        }
                    }
                }
            }
        }
    }

    void set_globBuilderFields(const PlayerView& playerView) {
        globBestNeighbor.assign(mapSize, vector<int>(mapSize, -1));
        globBestFields.clear();
        for (int f_y = 0; f_y < mapSize; f_y++) {
            for (int f_x = 0; f_x < mapSize; f_x++) {
                char f_typ = globGrid.at(f_y).at(f_x).first;
                if (f_typ != '.' && f_typ != 'B') {
                    continue;
                }
                bool has_free_nei = false;
                for (int fn = 0; fn < 4; fn++) {
                    int fn_y = f_y + directions.at(fn).first, fn_x = f_x + directions.at(fn).second;
                    if (is_valid(fn_y, fn_x)) {
                        char fn_typ = globGrid.at(fn_y).at(fn_x).first;
                        if (fn_typ == '.' || fn_typ == 'B') {
                            has_free_nei = true;
                            break;
                        }
                    }
                }
                if (f_typ == '.' && !has_free_nei) {
                    continue;
                }
                int best_nei = -1;
                double best_val = -1;
                for (int r = 0; r < 4; r++) {
                    int r_y = f_y + directions.at(r).first, r_x = f_x + directions.at(r).second;
                    if (!is_valid(r_y, r_x) || globGrid.at(r_y).at(r_x).first != '#' || !globCurrentVisible.at(r_y).at(r_x)) {
                        continue;
                    }
                    double res_val = 0;
                    for (int n = 0; n < 4; n++) {
                        int n_y = r_y + directions.at(n).first, n_x = r_x + directions.at(n).second;
                        if (!is_valid(n_y, n_x) || globGrid.at(n_y).at(n_x).first != '#' || !globCurrentVisible.at(n_y).at(n_x)) {
                            continue;
                        }
                        res_val += playerView.entities.at(globCoorToEntry.at({ n_y, n_x })).health;
                    }
                    for (int d = 0; d < 4; d++) {
                        int d_y = r_y + diag_neighbors.at(d).first, d_x = r_x + diag_neighbors.at(d).second;
                        if (!is_valid(d_y, d_x) || globGrid.at(d_y).at(d_x).first != '#' || !globCurrentVisible.at(d_y).at(d_x)) {
                            continue;
                        }
                        res_val += DIAG_MUL * playerView.entities.at(globCoorToEntry.at({ d_y, d_x })).health;
                    }
                    if (res_val > best_val) {
                        best_val = res_val;
                        best_nei = r;
                    }
                }
                if (best_nei == -1) // No neighboring resource
                    continue;
                double direct_neis_val = 0;
                for (int n = 0; n < 4; n++) {
                    int r_y = f_y + directions.at(n).first, r_x = f_x + directions.at(n).second;
                    if (!is_valid(r_y, r_x) || globGrid.at(r_y).at(r_x).first != '#' || !globCurrentVisible.at(r_y).at(r_x)) {
                        continue;
                    }
                    direct_neis_val += playerView.entityProperties.at(RESOURCE).maxHealth; //  playerView.entities.at(globCoorToPos.at({ r_y, r_x })).health;
                }
                for (int d = 0; d < 4; d++) {
                    int d_y = f_y + diag_neighbors.at(d).first, d_x = f_x + diag_neighbors.at(d).second;
                    if (!is_valid(d_y, d_x) || globGrid.at(d_y).at(d_x).first != '#' || !globCurrentVisible.at(d_y).at(d_x)) {
                        continue;
                    }
                    direct_neis_val += DIAG_MUL * playerView.entityProperties.at(RESOURCE).maxHealth; //playerView.entities.at(globCoorToPos.at({ d_y, d_x })).health;
                }

                globBestNeighbor.at(f_y).at(f_x) = best_nei;
                globBestFields.push_back({ resourceVal() * (best_val + direct_neis_val) - DANGER_MUL * globDangerFields.at(f_y).at(f_x) - f_y - f_x, {f_y, f_x} });
            }
        }
        auto lmb = [&](pair<double, pair<int, int>> p1, pair<double, pair<int, int>> p2) {return p1.first > p2.first; };
        std::sort(globBestFields.begin(), globBestFields.end(), lmb);
        for (int i = 0; i < globBestFields.size(); i++) {
            globBestFields.at(i).first -= globBestFields.back().first;
        }
        return;
    }

    void print_grid(const Grid& grid) {
        assert(grid.size() != 0);
        for (int y = grid.size() / 2 - 1; y >= 0; y--) {
            for (int x = 0; x < grid.size() / 2; x++) {
                std::cout << grid.at(y).at(x).first;
                if (grid.at(y).at(x).second == -1)
                    std::cout << " ";
                else if (grid.at(y).at(x).second == 0)
                    std::cout << "#";
                else
                    std::cout << grid.at(y).at(x).second;
            }
            std::cout << "\n";
        }
        sleep(200);
    }

    void print_grid(vector<vector<bool>> grid) {
        for (int y = mapSize / 2 - 1; y >= 0; y--) {
            for (int x = 0; x < mapSize / 2; x++) {
                if (grid.at(y).at(x) == 0)
                    std::cout << '.';
                else
                    std::cout << grid.at(y).at(x);
            }
            std::cout << "\n";
        }
    }

    void print_grid(vector<vector<int>> grid) {
        for (int y = mapSize - 1; y >= 0; y--) {
            for (int x = 0; x < mapSize; x++) {
                if (grid.at(y).at(x) == 9)
                    std::cout << '.';
                else
                    std::cout << grid.at(y).at(x);
                std::cout << " ";
            }
            std::cout << "\n";
        }
    }

    void print_unit_diff() { // Prints difference between nearby units
        for (int y = mapSize * 0.6 - 1; y >= 0; y--) {
            for (int x = 0; x < mapSize * 0.6; x++) {
                int diff = close_own_non_builders.at(y).at(x) - close_opponent_fighters.at(y).at(x);
                if (diff == 0) {
                    std::cout << "  .";
                }
                else {
                    if (diff < -9) {

                    }
                    else if (diff < 0 || diff >= 10)
                        std::cout << " ";
                    else
                        std::cout << "  ";
                    std::cout << diff;
                }
            }
            std::cout << "\n";
        }
    }

    void debug_grids() { // Print selected grids to std::cerr
        if (globDebug && PRINT_GRID)
            print_grid(globGrid);
        if (globDebug && PRINT_VISIBLE)
            print_grid(globCurrentVisible);
        if (globDebug && PRINT_RETREATS)
            print_grid(globRetreats);
        if (globDebug && PRINT_UNIT_DIFF)
            print_unit_diff();
        if (globDebug && PRINT_OPPONENT_RANGE) {
            print_grid(outside_opponent_range);
        }
    }
}

// Unit movement
namespace {
    void route_builders(const PlayerView& playerView) {
        globBestFields.reserve(globBestFields.size() + globBuildingFields.size());
        globBestFields.insert(globBestFields.end(), globBuildingFields.begin(), globBuildingFields.end());
        auto lmb = [&](pair<double, pair<int, int>> p1, pair<double, pair<int, int>> p2) {return p1.first > p2.first; };
        std::sort(globBestFields.begin(), globBestFields.end(), lmb);
        std::map<pair<int, int>, double> mp;
        for (auto [val, pos] : globBestFields) { // Remove duplicate positions
            mp.insert({ pos, val });
        }
        globBestFields.clear();
        for (auto [pos, val] : mp) {
            globBestFields.push_back({ val, pos });
        }
        std::sort(globBestFields.begin(), globBestFields.end(), lmb);
        if (globBuilderPositions.size() > globBestFields.size()) { // Too many builders
            globBuilderPositions.resize(globBestFields.size());
        }
        int N = globBuilderPositions.size();
        int M = min(2 * N + 800, (int)globBestFields.size());
        globBestFields.resize(M);
        vector<vector<double>> values(N, vector<double>(M));
        for (int b = 0; b < N; b++) {
            int b_y = globBuilderPositions.at(b).first, b_x = globBuilderPositions.at(b).second;
            for (int f = 0; f < M; f++) {
                int f_y = globBestFields.at(f).second.first, f_x = globBestFields.at(f).second.second;
                double dist = globDistances.at(b_y).at(b_x).at(f_y).at(f_x);
                if (dist == -1) // not reachable
                    dist = 9999;
                values.at(b).at(f) = -(globBestFields.at(f).first - builderDistMul() * dist + 0.1 * (dist > 0));
            }
        }
        globWillOccupy.assign(mapSize, vector<bool>(mapSize, false));
        vector<int> matchingPos(N, -1);
        get_minAssignment(values, matchingPos, vector<int>(M, -1));
        for (int b = 0; b < N; b++) {
            int b_y = globBuilderPositions.at(b).first, b_x = globBuilderPositions.at(b).second;
            int f_y = globBestFields.at(matchingPos.at(b)).second.first, f_x = globBestFields.at(matchingPos.at(b)).second.second;
            std::shared_ptr<MoveAction> moveAction(new MoveAction(Vec2Int(f_x, f_y), true, false));
            if (b_y == f_y && b_x == f_x) { // Arrived at destination
                if (globBuildBuildingPosition.at(b_y).at(b_x).first == -1) { // Mine resource
                    globMyResourceLeft++;
                    int r_y = f_y + directions.at(globBestNeighbor.at(f_y).at(f_x)).first, r_x = f_x + directions.at(globBestNeighbor.at(f_y).at(f_x)).second;
                    globResult.entityActions[globCoorToGameId.at({ b_y, b_x })] = EntityAction(
                        moveAction, nullptr,
                        std::shared_ptr<AttackAction>(new AttackAction(
                            std::shared_ptr<int>(new int(globCoorToGameId.at({ r_y, r_x }))), // Attack resource
                            nullptr)),
                        nullptr);
                }
                else { // Building
                    auto [h_y, h_x] = globBuildBuildingPosition.at(b_y).at(b_x);
                    std::shared_ptr<BuildAction> buildAction(new BuildAction(globBuildBuildingType.at(b_y).at(b_x), Vec2Int(h_x, h_y)));
                    std::shared_ptr<RepairAction> repairAction = nullptr;
                    if (globCoorToGameId.count({ h_y, h_x })) {
                        repairAction = std::shared_ptr<RepairAction>(new RepairAction(globCoorToGameId.at({ h_y, h_x })));
                    }
                    globResult.entityActions[globCoorToGameId.at({ b_y, b_x })] = EntityAction(nullptr, buildAction, nullptr, repairAction);
                }
            }
            else {
                globResult.entityActions[globCoorToGameId.at({ b_y, b_x })] = EntityAction(moveAction, nullptr, nullptr, nullptr);
            }
            globWillOccupy.at(f_y).at(f_x) = true;
            globActionDone.at(globCoorToEntry.at({ globBuilderPositions.at(b).first, globBuilderPositions.at(b).second })) = true;
        }
    }

    void set_fighter_targets(const PlayerView& playerView) {
        std::multimap<int, pair<int, int>> closest_opponent_units;
        vector<pair<pair<int, int>, int>> own_rangeds, own_melees;
        vector<pair<int, int>> ranged_targets, melee_targets;
        for (size_t i = 0; i < playerView.entities.size(); i++) {
            const Entity& entity = playerView.entities[i];
            if (entity.playerId == nullptr) {
                continue;
            }
            int u_y = entity.position.y, u_x = entity.position.x;
            if (is_opponent_unit(entity)) {
                closest_opponent_units.insert({ u_y + u_x, {u_y, u_x} });
            }
            else if (is_own_fighter(entity)) {
                if (entity.entityType == MELEE_UNIT) {
                    own_melees.push_back({ {u_y, u_x}, (int)i });
                }
                else {
                    own_rangeds.push_back({ {u_y, u_x}, (int)i });
                }
            }
        }
        if (closest_opponent_units.size() < own_rangeds.size()) { // Attack buildings
            for (size_t i = 0; i < playerView.entities.size(); i++) {
                const Entity& entity = playerView.entities[i];
                if (entity.playerId == nullptr || *entity.playerId == globMyId) {
                    continue;
                }
                int u_y = entity.position.y, u_x = entity.position.x;
                EntityType typ = entity.entityType;
                if (typ == MELEE_BASE || typ == RANGED_BASE || typ == BUILDER_BASE || typ == HOUSE || typ == WALL || typ == TURRET) {
                    closest_opponent_units.insert({ u_y + u_x, {u_y, u_x} });
                }
            }
        }
        int o_count = 0;
        for (auto opponent_unit : closest_opponent_units) {
            if (o_count < own_rangeds.size() && ranged_targets.size() < own_rangeds.size()) {
                ranged_targets.push_back(opponent_unit.second);
                if (o_count < 3 && 2ll * o_count + 1ll < own_rangeds.size() && ranged_targets.size() < own_rangeds.size()) {
                    ranged_targets.push_back(opponent_unit.second);
                }
            }
            if (o_count++ < own_melees.size()) {
                melee_targets.push_back(opponent_unit.second);
            }
        }
        std::sort(ranged_targets.begin(), ranged_targets.end(), angle_cmp);
        std::sort(own_rangeds.begin(), own_rangeds.end(), second_angle_cmp);
        std::sort(melee_targets.begin(), melee_targets.end(), angle_cmp);
        std::sort(own_melees.begin(), own_melees.end(), second_angle_cmp);
        for (int u = 0; u < ranged_targets.size(); u++) {
            std::shared_ptr<MoveAction> moveAction = std::shared_ptr<MoveAction>(new MoveAction(
                Vec2Int(ranged_targets.at(u).second, ranged_targets.at(u).first), true, true));
            globResult.entityActions[playerView.entities.at(own_rangeds.at(u).second).id] = EntityAction(
                moveAction, nullptr,
                std::shared_ptr<AttackAction>(new AttackAction(
                    nullptr, std::shared_ptr<AutoAttack>(new AutoAttack(5, vector<EntityType>())))),
                nullptr);
            globActionDone.at(own_rangeds.at(u).second) = true;
        }
        for (int u = 0; u < melee_targets.size(); u++) {
            std::shared_ptr<MoveAction> moveAction = std::shared_ptr<MoveAction>(new MoveAction(
                Vec2Int(melee_targets.at(u).second, melee_targets.at(u).first), true, true));
            globResult.entityActions[playerView.entities.at(own_melees.at(u).second).id] = EntityAction(
                moveAction, nullptr,
                std::shared_ptr<AttackAction>(new AttackAction(
                    nullptr, std::shared_ptr<AutoAttack>(new AutoAttack(5, vector<EntityType>())))),
                nullptr);
            globActionDone.at(own_melees.at(u).second) = true;
        }
    }

    void retreat_overwrite(const PlayerView& playerView) {
        auto& mapSize = playerView.mapSize;
        for (int g_y = 0; g_y < mapSize; g_y++) {
            for (int g_x = 0; g_x < mapSize; g_x++) {
                char typ = globGrid.at(g_y).at(g_x).first;
                if ((globRetreats.at(g_y).at(g_x) || globStays.at(g_y).at(g_x)) && (typ == 'M' || typ == 'R' || typ == 'B')) {
                    set_retreat(g_y, g_x);
                }
            }
        }
    }

    void set_attackTargets(const PlayerView& playerView) {
        vector<vector<int>> myRangersInRange(mapSize, vector<int>(mapSize, 0)); // for deciding attack priority
        for (int g_y = 0; g_y < mapSize; g_y++) {
            for (int g_x = 0; g_x < mapSize; g_x++) {
                const auto& [g_typ, g_owner] = globGrid.at(g_y).at(g_x);
                int range = 5;
                if (g_owner != globMyId && g_owner > 0) { // opponent entity
                    for (int r_y = max(0, g_y - range); r_y < min(mapSize, g_y + range + 1); r_y++) {
                        int y_dist = abs(r_y - g_y);
                        int x_max = range - y_dist;
                        for (int r_x = max(0, g_x - x_max); r_x < min(mapSize, g_x + x_max + 1); r_x++) {
                            const auto& [r_typ, r_owner] = globGrid.at(g_y).at(g_x);
                            if (r_typ == 'R' && r_owner == globMyId) {
                                myRangersInRange.at(g_y).at(g_x)++;
                            }
                        }
                    }
                }
            }
        }
        std::map<pair<int, int>, int> o_pos_to_id;
        std::map<int, pair<int, int>> o_id_to_pos;
        std::map<int, pair<int, int>> m_id_to_pos;
        vector<vector<double>> costs(MAX_OWN_UNITS, vector<double>(1000));
        int N = 0, M = 0;
        for (int i = 0; i < playerView.entities.size(); i++) {
            const Entity& entity = playerView.entities.at(i);
            auto [ent_x, ent_y] = entity.position;
            const auto& field = globGrid.at(ent_y).at(ent_x);
            int range;
            if (field == make_pair('R', globMyId)) {
                range = 5;
            }
            else if (field == make_pair('M', globMyId)) {
                range = 1;
            }
            else {
                continue;
            }
            if (globRetreats.at(ent_y).at(ent_x)) {
                continue;
            }
            bool has_attack_target = false;
            for (int o_y = max(0, ent_y - range); o_y < min(mapSize, ent_y + range + 1); o_y++) {
                int y_dist = abs(o_y - ent_y);
                int x_max = range - y_dist;
                for (int o_x = max(0, ent_x - x_max); o_x < min(mapSize, ent_x + x_max + 1); o_x++) {
                    auto [typ, owner] = globGrid.at(o_y).at(o_x);
                    if (owner != globMyId && owner > 0) { // opponent
                        has_attack_target = true;
                        int o_health = playerView.entities.at(globCoorToEntry.at({ o_y, o_x })).health;
                        int o_tank = (o_health + 4) / 5;
                        int o_id;
                        if (o_pos_to_id.count({ o_y, o_x })) {
                            o_id = o_pos_to_id.at({ o_y, o_x });
                        }
                        else {
                            o_pos_to_id.insert({ { o_y, o_x }, M });
                            for (int m = 0; m < o_tank; m++) {
                                o_id_to_pos.insert({ M + m, {o_y, o_x} });
                            }
                            o_id = M;
                            M += o_tank;
                        }
                        double kill_value;
                        if (typ == 'R') {
                            kill_value = 1000.0;
                        }
                        else if (typ == 'M') {
                            kill_value = 500.0;
                        }
                        else if (typ == 'B') {
                            kill_value = 300.0;
                        }
                        else {
                            kill_value = 200.0;
                        }
                        for (int m = 0; m < o_tank; m++) {
                            costs.at(N).at((ULL)o_id + m) = ((double)o_y + o_x) / 10.0 - kill_value - myRangersInRange.at(o_y).at(o_x) + o_tank + (globCoorToEntry.at({ o_y, o_x }) % 1000) / 10000.0;
                        }
                    }
                }
            }
            if (has_attack_target) {
                m_id_to_pos.insert({ N, {ent_y, ent_x} });
                N++;
            }
        }
        costs.resize(N);
        for (int n = 0; n < N; n++) {
            costs.at(n).resize(max(N, M)); // max for case where less opponent targets than attackers
        }
        vector<int> L(N, -1);
        get_minAssignment(costs, L, vector<int>(max(N, M), -1)); // max for case where less opponent targets than attackers
        for (int n = 0; n < N; n++) {
            if (L.at(n) < M) {
                std::shared_ptr<AttackAction> attackAction(std::shared_ptr<AttackAction>(new AttackAction(std::shared_ptr<int>(new int(globCoorToGameId.at(o_id_to_pos.at(L.at(n))))), nullptr)));
                globResult.entityActions[globCoorToGameId.at(m_id_to_pos.at(n))] = {
                    nullptr,
                    nullptr,
                    attackAction,
                    nullptr
                };
                globActionDone.at(globCoorToEntry.at(m_id_to_pos.at(n))) = true;
            }
        }
    }
}

// Recruiting, Building
namespace {
    void set_globBuilderPositions(const PlayerView& playerView) {
        globBuilderPositions.clear();
        for (size_t i = 0; i < playerView.entities.size(); i++) {
            if (globActionDone.at(i)) {
                continue;
            }
            const Entity& entity = playerView.entities[i];
            int ent_y = entity.position.y, ent_x = entity.position.x;
            int owner = entity.playerId == nullptr ? 0 : *entity.playerId;
            if (entity.entityType == BUILDER_UNIT && owner == globMyId) {
                globBuilderPositions.push_back({ ent_y, ent_x });
            }
        }
    }

    void set_globDangerLevel(const Grid& grid) {
        globDangerLevel = 0;
        double furthest = 0.6 * grid.size();
        for (int g_y = 0; g_y < furthest; g_y++) {
            for (int g_x = 0; (LL)g_y + g_x < furthest; g_x++) {
                const pair<char, int>& field = grid.at(g_y).at(g_x);
                if (field.second > 0 && field.second != globMyId) { // opponent unit
                    double mult = 1;
                    if (field.first == 'M' || field.first == 'B') {
                        mult = 2;
                    }
                    globDangerLevel += mult * (furthest - ((LL)g_y + g_x));
                }
            }
        }
    }

    void set_lowestDistanceRecruitment(int ent_y, int ent_x, int ent_sz, int& best_y, int& best_x) {
        for (int y_o = 0; y_o < ent_sz; y_o++) {
            int r_y = ent_y + y_o, r_x = ent_x - 1;
            if (is_valid(r_y, r_x) && globGrid.at(r_y).at(r_x).first == '.') {
                if (best_y == -1 || globClosestFreeResourceDistance.at(r_y).at(r_x) <= globClosestFreeResourceDistance.at(best_y).at(best_x)) {
                    best_y = r_y, best_x = r_x;
                }
            }
            r_x = ent_x + ent_sz;
            if (is_valid(r_y, r_x) && globGrid.at(r_y).at(r_x).first == '.') {
                if (best_y == -1 || globClosestFreeResourceDistance.at(r_y).at(r_x) <= globClosestFreeResourceDistance.at(best_y).at(best_x)) {
                    best_y = r_y, best_x = r_x;
                }
            }
        }
        for (int x_o = 0; x_o < ent_sz; x_o++) {
            int r_y = ent_y - 1, r_x = ent_x + x_o;
            if (is_valid(r_y, r_x) && globGrid.at(r_y).at(r_x).first == '.') {
                if (best_y == -1 || globClosestFreeResourceDistance.at(r_y).at(r_x) <= globClosestFreeResourceDistance.at(best_y).at(best_x)) {
                    best_y = r_y, best_x = r_x;
                }
            }
            r_y = ent_y + ent_sz, r_x = ent_x + x_o;
            if (is_valid(r_y, r_x) && globGrid.at(r_y).at(r_x).first == '.') {
                if (best_y == -1 || globClosestFreeResourceDistance.at(r_y).at(r_x) <= globClosestFreeResourceDistance.at(best_y).at(best_x)) {
                    best_y = r_y, best_x = r_x;
                }
            }
        }
    }

    void set_factoryRecruitment(const PlayerView& playerView, const Grid& grid, int& myResourceLeft) {
        for (size_t i = 0; i < playerView.entities.size(); i++) {
            const Entity& entity = playerView.entities[i];
            if (entity.playerId == nullptr || *entity.playerId != globMyId || is_own_unit(entity)) {
                continue;
            }
            const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
            if (properties.build == nullptr) {
                continue;
            }
            EntityType entityType = properties.build->options[0];
            if (globMyTotalPopulation + 1 <= globMyCurrentPossiblePopulation && myResourceLeft >= playerView.entityProperties.at(entityType).initialCost &&
                ((entityType == BUILDER_UNIT && (double)globMyBuilderPopulation / (double)globMyTotalPopulation <= globBuilderShare)
                    || (entityType == RANGED_UNIT && (double)globMyRangedPopulation / (double)globMyTotalPopulation <= globRangedShare))) {
                int best_y = -1, best_x = -1;
                set_lowestDistanceRecruitment(entity.position.y, entity.position.x, properties.size, best_y, best_x);
                if (best_y != -1) {
                    globMyTotalPopulation++;
                    if (entityType == BUILDER_UNIT) {
                        myResourceLeft -= globMyBuilderPopulation;
                        globMyBuilderPopulation++;
                    }
                    else if (entityType == MELEE_UNIT) {
                        myResourceLeft -= globMyMeleePopulation;
                        globMyMeleePopulation++;
                    }
                    else if (entityType == RANGED_UNIT) {
                        myResourceLeft -= globMyRangedPopulation;
                        globMyRangedPopulation++;
                    }
                    myResourceLeft -= playerView.entityProperties.at(entityType).initialCost;
                    std::shared_ptr<BuildAction> buildAction = std::shared_ptr<BuildAction>(new BuildAction(
                        entityType,
                        Vec2Int(best_x, best_y)));
                    globResult.entityActions[entity.id] = EntityAction(
                        nullptr,
                        buildAction,
                        std::shared_ptr<AttackAction>(new AttackAction(
                            nullptr, std::shared_ptr<AutoAttack>(new AutoAttack(properties.sightRange, vector<EntityType>())))),
                        nullptr);
                    globActionDone.at(i) = true;
                }
            }
        }
    }

    void set_houseBuilding(const PlayerView& playerView, const Grid& grid, int& myResourceLeft) {
        std::set<pair<EntityType, pair<int, int>>> built;
        std::map<pair<EntityType, pair<int, int>>, int> constructing;
        std::set<pair<EntityType, pair<int, int>>> obstructed;
        for (unsigned h = 0; h < globPossibleBuildings.size(); h++) {
            EntityType typ = globPossibleBuildings.at(h).type;
            const EntityProperties& properties = playerView.entityProperties.at(globPossibleBuildings.at(h).type);
            int h_y = globPossibleBuildings.at(h).pos.first;
            int h_x = globPossibleBuildings.at(h).pos.second;
            for (size_t y = h_y; y < (ULL)h_y + properties.size; y++) {
                for (size_t x = h_x; x < (ULL)h_x + properties.size; x++) {
                    if (grid.at(y).at(x).second != -1) {
                        obstructed.insert({ typ, { h_y, h_x } });
                        y = (ULL)1e9; break;
                    }
                }
            }
        }
        std::map<pair<int, int>, int> builder_indices;

        bool building_rangedBase = false, building_meleeBase = false;
        bool planned_rangedBase = false, planned_meleeBase = false;
        for (size_t i = 0; i < playerView.entities.size(); i++) {
            const Entity& entity = playerView.entities[i];
            EntityType typ = entity.entityType;
            if (entity.playerId == nullptr || *entity.playerId != globMyId) {
                continue;
            }
            const EntityProperties properties = playerView.entityProperties.at(entity.entityType);
            if (entity.entityType == HOUSE || entity.entityType == RANGED_BASE || entity.entityType == MELEE_BASE || entity.entityType == TURRET) {
                if (entity.health == properties.maxHealth) {
                    built.insert({ typ, { entity.position.y, entity.position.x } });
                }
                else {
                    if (entity.entityType == RANGED_BASE) {
                        building_rangedBase = true;
                    }
                    else if (entity.entityType == MELEE_BASE) {
                        building_meleeBase = true;
                    }
                    constructing.insert({ { typ, { entity.position.y, entity.position.x } }, (int)i });
                }
            }
            else if (entity.entityType == BUILDER_UNIT) {
                builder_indices.insert({ {entity.position.y, entity.position.x}, (int)i });
            }
        }
        globBuiltBuildings.resize(globPossibleBuildings.size(), false);
        globIncompleteBuildings.assign(globPossibleBuildings.size(), false);
        globFreeBuildings.assign(globPossibleBuildings.size(), false);
        globTreatedBuildings.assign(globPossibleBuildings.size(), false);
        for (size_t b = 0; b < globPossibleBuildings.size(); b++) {
            EntityType typ = globPossibleBuildings.at(b).type;
            if (built.count({ typ, globPossibleBuildings.at(b).pos })) {
                assert(obstructed.count({ typ, globPossibleBuildings.at(b).pos }));
                assert(!constructing.count({ typ, globPossibleBuildings.at(b).pos }));
                globBuiltBuildings.at(b) = true;
            }
            if (constructing.count({ typ, globPossibleBuildings.at(b).pos })) {
                assert(obstructed.count({ typ, globPossibleBuildings.at(b).pos }));
                assert(!built.count({ typ, globPossibleBuildings.at(b).pos }));
                globIncompleteBuildings.at(b) = true;
            }
            if (!obstructed.count({ typ, globPossibleBuildings.at(b).pos })) {
                assert(!built.count({ typ, globPossibleBuildings.at(b).pos }));
                assert(!constructing.count({ typ, globPossibleBuildings.at(b).pos }));
                globFreeBuildings.at(b) = true;
            }
        }
        bool newBuild = true;
        int nBuilt = 0;
        globBuildBuildingPosition.assign(mapSize, vector<pair<int, int>>(mapSize, { -1, -1 }));
        globBuildBuildingType.assign(mapSize, vector<EntityType>(mapSize));
        globBuildingFields.clear();
        globPlannedPos.assign(mapSize, vector<pair<int, int>>(mapSize, { -1, -1 }));
        globMyPlannedPossiblePopulation = globMyCurrentPossiblePopulation;
        while (newBuild) {
            newBuild = false;
            vector<std::multimap<int, int>> cheap_labor(globPossibleBuildings.size());
            for (auto& worker : builder_indices) {
                int w_y = worker.first.first, w_x = worker.first.second, w_id = worker.second;
                if (globActionDone.at(w_id))
                    continue;
                for (unsigned b = 0; b < globPossibleBuildings.size(); b++) {
                    int h_y = globPossibleBuildings.at(b).pos.first + 1, h_x = globPossibleBuildings.at(b).pos.second + 1;
                    cheap_labor.at(b).insert({ abs(w_y - h_y) + abs(w_x - h_x), w_id });
                    if (cheap_labor.at(b).size() > 3) {
                        cheap_labor.at(b).erase(--cheap_labor.at(b).end());
                    }
                }
            }
            int best_val = 1e9, best_id = -1;
            for (int h = 0; h < globFreeBuildings.size(); h++) {
                auto [h_y, h_x] = globPossibleBuildings.at(h).pos;
                if (outside_opponent_range.at(h_y).at(h_x) < 9 || outside_opponent_range.at(h_y).at(h_x + 2ll) < 9 ||
                    outside_opponent_range.at(h_y + 2ll).at(h_x) < 9 || outside_opponent_range.at(h_y + 2ll).at(h_x + 2ll) < 9) {
                    continue; // Don't build house close to opponent
                }
                if (globFreeBuildings.at(h) || globIncompleteBuildings.at(h)) { // Only consider building when possible to build
                    int labor_sum = HOUSE_NUMBER_MUL * h;
                    for (auto cost : cheap_labor.at(h))
                        labor_sum += cost.first;
                    if (!globTreatedBuildings.at(h) && ((globFreeBuildings.at(h) && myResourceLeft >=
                        playerView.entityProperties.at(globPossibleBuildings.at(h).type).initialCost + (LABOR_SUM_MULTIMPLIER * labor_sum + nBuilt * SUB_FOR_BUILT) * (0.05 + ((double)globMyPlannedPossiblePopulation - globMyTotalPopulation) / 12.0)) ||
                        globIncompleteBuildings.at(h))) {
                        if (labor_sum < best_val || (globIncompleteBuildings.at(h) && labor_sum < best_val + 30)) { // Still choose incomplete over best
                            EntityType typ = globPossibleBuildings.at(h).type;
                            if ((typ == RANGED_BASE && globMyRangedBases == 0 && (!building_rangedBase || globIncompleteBuildings.at(h)) && !planned_rangedBase)
                                || (typ == HOUSE && (building_rangedBase || globMyTotalHouses + (int)globFreeBuildings.at(h) <= HOUSES_BEFORE_BASE || globMyRangedBases >= 1))) { // Guarantee ranged base before many houses
                                best_val = labor_sum;
                                best_id = h;
                            }
                        }
                    }
                }
            }
            if (best_id == -1) {
                if (globDebug) { std::cout << "No good building to build\n"; }
                break;
            }
            else if (cheap_labor.at(best_id).size() == 0) {
                if (globDebug) { std::cout << "All builders officially building\n"; }
                break;
            }
            else {
                globTreatedBuildings.at(best_id) = 1;
                newBuild = true;
            }
            const EntityType& buildingType = globPossibleBuildings.at(best_id).type;
            assert(globIncompleteBuildings.at(best_id) || globFreeBuildings.at(best_id));
            globMyPlannedPossiblePopulation += playerView.entityProperties.at(buildingType).populationProvide;
            if (globFreeBuildings.at(best_id)) {
                assert(!obstructed.count({ buildingType, globPossibleBuildings.at(best_id).pos }));
                assert(!constructing.count({ buildingType, globPossibleBuildings.at(best_id).pos }));
                myResourceLeft -= playerView.entityProperties.at(globPossibleBuildings.at(best_id).type).initialCost;
                nBuilt++;
            }
            const auto& [ent_y, ent_x] = globPossibleBuildings.at(best_id).pos;
            const int ent_sz = playerView.entityProperties.at(buildingType).size;
            if (!constructing.count({ buildingType, globPossibleBuildings.at(best_id).pos })) {
                for (int y_o = 0; y_o < ent_sz; y_o++) {
                    for (int x_o = 0; x_o < ent_sz; x_o++) {
                        if (buildingType == RANGED_BASE) {
                            globGrid.at(ent_y + y_o).at(ent_x + x_o).first = '!';
                        }
                        else if (buildingType == MELEE_BASE) {
                            globGrid.at(ent_y + y_o).at(ent_x + x_o).first = '%';
                        }
                        else if (buildingType == HOUSE) {
                            globGrid.at(ent_y + y_o).at(ent_x + x_o).first = 'h';
                        }
                        else {
                            globGrid.at(ent_y + y_o).at(ent_x + x_o).first = '?';
                        }
                        globGrid.at(ent_y + y_o).at(ent_x + x_o).second = globMyId;
                        globPlannedPos.at(ent_y + y_o).at(ent_x + x_o) = { ent_y, ent_x };
                    }
                }
            }
            if (buildingType == HOUSE) {
                globMyTotalHouses++;
                globBuildingFields.push_back({ houseBuilderValue(0), {globPossibleBuildings.at(best_id).n1} });
                const auto& [n1_y, n1_x] = globPossibleBuildings.at(best_id).n1;
                globBuildBuildingPosition.at(n1_y).at(n1_x) = globPossibleBuildings.at(best_id).pos;
                globBuildBuildingType.at(n1_y).at(n1_x) = globPossibleBuildings.at(best_id).type;

                globBuildingFields.push_back({ houseBuilderValue(1), {globPossibleBuildings.at(best_id).n2} });
                const auto& [n2_y, n2_x] = globPossibleBuildings.at(best_id).n2;
                globBuildBuildingPosition.at(n2_y).at(n2_x) = globPossibleBuildings.at(best_id).pos;
                globBuildBuildingType.at(n2_y).at(n2_x) = globPossibleBuildings.at(best_id).type;

                globBuildingFields.push_back({ houseBuilderValue(2), {globPossibleBuildings.at(best_id).n3} });
                const auto& [n3_y, n3_x] = globPossibleBuildings.at(best_id).n3;
                globBuildBuildingPosition.at(n3_y).at(n3_x) = globPossibleBuildings.at(best_id).pos;
                globBuildBuildingType.at(n3_y).at(n3_x) = globPossibleBuildings.at(best_id).type;
            }
            else if (buildingType == RANGED_BASE) {
                planned_rangedBase = true;
                set_allAdjacentBuildingFields(playerView, best_id);
            }
            else {
                assert(0);
            }
        }
    }

    void repair_wounded_units(const PlayerView& playerView, const Grid& grid) {
        for (size_t i = 0; i < playerView.entities.size(); i++) {
            const Entity& entity = playerView.entities[i];
            if (entity.playerId == nullptr || *entity.playerId != globMyId || entity.entityType != BUILDER_UNIT) {
                continue;
            }
            const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);
            int ent_y = entity.position.y, ent_x = entity.position.x;

            // Heal low safe archer
            for (int d = 0; d < 4; d++) {
                int c_y = ent_y + directions.at(d).first, c_x = ent_x + directions.at(d).second;
                if (is_valid(c_y, c_x)) {
                    char c_typ = globGrid.at(c_y).at(c_x).first;
                    char owner = globGrid.at(c_y).at(c_x).second;
                    if (c_typ == 'R' && owner == globMyId) {
                        const Entity& c_entity = playerView.entities.at(globCoorToEntry.at({ c_y, c_x }));
                        if (c_entity.health != playerView.entityProperties.at(c_entity.entityType).maxHealth && globInOpponentsRange.at(c_y).at(c_x) == 0) {
                            std::shared_ptr<RepairAction> repairAction(new RepairAction(globCoorToGameId.at({ c_y, c_x })));
                            globResult.entityActions[entity.id] = EntityAction(
                                nullptr,
                                nullptr,
                                nullptr,
                                repairAction);
                            globActionDone.at(i) = true;
                            break;
                        }
                    }
                }
            }
            // Heal when health % 5 == 0
            for (int d = 0; d < 4; d++) {
                int c_y = ent_y + directions.at(d).first, c_x = ent_x + directions.at(d).second;
                if (is_valid(c_y, c_x)) {
                    char c_typ = globGrid.at(c_y).at(c_x).first;
                    char owner = globGrid.at(c_y).at(c_x).second;
                    if ((c_typ == 'M' || c_typ == 'R' || c_typ == 'B') && owner == globMyId) {
                        const Entity& c_entity = playerView.entities.at(globCoorToEntry.at({ c_y, c_x }));
                        if (c_entity.health != playerView.entityProperties.at(c_entity.entityType).maxHealth && c_entity.health % 5 == 0 && (globInOpponentsRange.at(c_y).at(c_x) == 0 || c_entity.health != 5)) {
                            std::shared_ptr<RepairAction> repairAction(new RepairAction(globCoorToGameId.at({ c_y, c_x })));
                            globResult.entityActions[entity.id] = EntityAction(
                                nullptr,
                                nullptr,
                                nullptr,
                                repairAction);
                            globActionDone.at(i) = true;
                            break;
                        }
                    }
                }
            }
            // Heal non-fighting archer
            for (int d = 0; d < 4; d++) {
                int c_y = ent_y + directions.at(d).first, c_x = ent_x + directions.at(d).second;
                if (is_valid(c_y, c_x)) {
                    char c_typ = globGrid.at(c_y).at(c_x).first;
                    char owner = globGrid.at(c_y).at(c_x).second;
                    if (c_typ == 'R' && owner == globMyId) {
                        const Entity& c_entity = playerView.entities.at(globCoorToEntry.at({ c_y, c_x }));
                        if (c_entity.health != playerView.entityProperties.at(c_entity.entityType).maxHealth && globInOpponentsRange.at(c_y).at(c_x) == 0) {
                            std::shared_ptr<RepairAction> repairAction(new RepairAction(globCoorToGameId.at({ c_y, c_x })));
                            globResult.entityActions[entity.id] = EntityAction(
                                nullptr,
                                nullptr,
                                nullptr,
                                repairAction);
                            globActionDone.at(i) = true;
                            break;
                        }
                    }
                }
            }
            // Heal fighting, surviving archer
            for (int d = 0; d < 4; d++) {
                int c_y = ent_y + directions.at(d).first, c_x = ent_x + directions.at(d).second;
                if (is_valid(c_y, c_x)) {
                    char c_typ = globGrid.at(c_y).at(c_x).first;
                    char owner = globGrid.at(c_y).at(c_x).second;
                    if (c_typ == 'R' && owner == globMyId) {
                        const Entity& c_entity = playerView.entities.at(globCoorToEntry.at({ c_y, c_x }));
                        if (c_entity.health > 5 && globInOpponentsRange.at(c_y).at(c_x) == 1) {
                            std::shared_ptr<RepairAction> repairAction(new RepairAction(globCoorToGameId.at({ c_y, c_x })));
                            globResult.entityActions[entity.id] = EntityAction(
                                nullptr,
                                nullptr,
                                nullptr,
                                repairAction);
                            globActionDone.at(i) = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    void guarantee_buildAdjacentBase(bool& newBuilt) {
        for (int g_y = 0; g_y < mapSize; g_y++) {
            for (int g_x = 0; g_x < mapSize; g_x++) {
                if (outside_opponent_range.at(g_y).at(g_x) >= 9) { // Only build when far from opponent
                    if (globGrid.at(g_y).at(g_x) == std::make_pair('B', globMyId)) {
                        bool should_build = false;
                        std::shared_ptr<BuildAction> buildAction = nullptr;
                        for (int n = 0; n < 4; n++) {
                            int n_y = g_y + directions.at(n).first, n_x = g_x + directions.at(n).second;
                            if (is_valid(n_y, n_x)) {
                                if (globGrid.at(n_y).at(n_x).first == '!') {
                                    should_build = true;
                                    buildAction = std::shared_ptr<BuildAction>(new BuildAction(RANGED_BASE,
                                        Vec2Int(globPlannedPos.at(n_y).at(n_x).second, globPlannedPos.at(n_y).at(n_x).first)));
                                    break;
                                }
                                else if (globGrid.at(n_y).at(n_x).first == '%') {
                                    should_build = true;
                                    buildAction = std::shared_ptr<BuildAction>(new BuildAction(MELEE_BASE,
                                        Vec2Int(globPlannedPos.at(n_y).at(n_x).second, globPlannedPos.at(n_y).at(n_x).first)));
                                    break;
                                }
                                else if (globGrid.at(n_y).at(n_x).first == 'h') {
                                    should_build = true;
                                    buildAction = std::shared_ptr<BuildAction>(new BuildAction(HOUSE,
                                        Vec2Int(globPlannedPos.at(n_y).at(n_x).second, globPlannedPos.at(n_y).at(n_x).first)));
                                }
                            }
                        }
                        if (should_build) {
                            newBuilt = true;
                            globResult.entityActions[globCoorToGameId.at({ g_y, g_x })] = {
                                nullptr,
                                buildAction,
                                nullptr,
                                nullptr
                            };
                        }
                    }
                }
            }
        }
    }
}

MyStrategy::MyStrategy() {}

Action MyStrategy::getAction(const PlayerView& playerView, DebugInterface* debugInterface)
{
    std::ios_base::sync_with_stdio(0);
    globActionDone.assign(playerView.entities.size(), false);
    globMyId = playerView.myId;
    globTick++;
    globMyResourceLeft = playerView.players.at(globMyId - 1ll).resource;
    globResult = Action(std::unordered_map<int, EntityAction>());
    if (!playerView.fogOfWar && globTick <= 1) {
        set_allSeen();
    }
    else if (playerView.fogOfWar) {
        set_currentVisible(playerView);
    }
    Grid oldGrid;
    set_grids(globGrid, oldGrid, playerView);
    set_globUnitCounts(playerView);
    set_unitShares();
    debug_grids();
    set_globDangerLevel(globGrid);
    set_globClosestResourceDistance();
    set_globDangerFields();
    set_globBuilderFields(playerView);
    adapt_globBuilderDistancesChanged(globNewBuilt || resource_removed(globGrid, oldGrid));
    set_globBuilderDistances();
    set_houseBuilding(playerView, globGrid, globMyResourceLeft);
    set_globBuilderPositions(playerView);
    set_fighter_targets(playerView);
    globMyRetreats = 0;
    route_builders(playerView);
    set_globClosestFreeResourceDistance();
    set_factoryRecruitment(playerView, globGrid, globMyResourceLeft);
    retreat_overwrite(playerView);
    set_attackTargets(playerView);
    repair_wounded_units(playerView, globGrid);
    globNewBuilt = false;
    guarantee_buildAdjacentBase(globNewBuilt); // TODO: reorder before set_globBuilderDistances() for own pathfinding
    //std::set<int> needsRepair;
    if (globDebug) {
        std::cerr << "Amount of entities: " << playerView.entities.size() << "\n";
        //std::cerr << "Needing repair: " << needsRepair.size() << "\n";
        std::cerr << "My resource: " << globMyResourceLeft << "\n";
        std::cerr << "My total population: " << globMyTotalPopulation << "\n";
        std::cerr << "My builder population: " << globMyBuilderPopulation << "\n";
        std::cerr << "My ranged population: " << globMyRangedPopulation << "\n";
        std::cerr << "My current possible population: " << globMyCurrentPossiblePopulation << "\n";
        std::cerr << "My planned possible population: " << globMyPlannedPossiblePopulation << "\n";
        std::cerr << "globDangerLevel: " << globDangerLevel << "\n";
        std::cerr << "Units retreating: " << globMyRetreats << "\n";
    }
    for (size_t i = 0; i < playerView.entities.size(); i++) {
        const Entity& entity = playerView.entities[i];
        int ent_y = entity.position.y, ent_x = entity.position.x;
        char typ = globGrid.at(ent_y).at(ent_x).first;
        if (globActionDone.at(i)) {
            continue;
        }
        if (entity.playerId == nullptr || *entity.playerId != globMyId) {
            continue;
        }
        const EntityProperties& properties = playerView.entityProperties.at(entity.entityType);


        std::shared_ptr<MoveAction> moveAction = nullptr;
        std::shared_ptr<BuildAction> buildAction = nullptr;
        std::shared_ptr<RepairAction> repairAction = nullptr;
        if (properties.canMove) {
            if (playerView.players.size() == 2 && globTick >= 200) {
                moveAction = std::shared_ptr<MoveAction>(new MoveAction(
                    Vec2Int(playerView.mapSize - 10, playerView.mapSize - 10),
                    true, true));
            }
            else {
                moveAction = std::shared_ptr<MoveAction>(new MoveAction(
                    Vec2Int(std::min(playerView.mapSize - 1, std::max(0, entity.position.x + rand() % 3 - 1 + (entity.position.x + entity.position.y < 23 + globTick / 25))),
                        std::min(playerView.mapSize - 1, std::max(0, entity.position.y + rand() % 3 - 1 + (entity.position.x + entity.position.y < 23 + globTick / 25)))),
                    true, true));
            }
        }
        else if (properties.build != nullptr) {
            EntityType entityType = properties.build->options[0];
            size_t currentUnits = 0;
            for (size_t j = 0; j < playerView.entities.size(); j++) {
                if (playerView.entities[j].playerId != nullptr && *playerView.entities[j].playerId == globMyId
                    && playerView.entities[j].entityType == entityType) {
                    currentUnits++;
                }
            }
        }
        vector<EntityType> validAutoAttackTargets;
        if (entity.entityType == BUILDER_UNIT) {
            if (globMyResourceLeft >= GlobResourceNeeded) {
                buildAction = std::shared_ptr<BuildAction>(new BuildAction(TURRET, Vec2Int(entity.position.x + 1, entity.position.y)));
                globMyResourceLeft -= playerView.entityProperties.at(TURRET).initialCost;
            }
            else {
                validAutoAttackTargets.push_back(RESOURCE);
            }
        }
        globResult.entityActions[entity.id] = EntityAction(
            moveAction,
            buildAction,
            std::shared_ptr<AttackAction>(new AttackAction(
                nullptr, std::shared_ptr<AutoAttack>(new AutoAttack(200, validAutoAttackTargets)))),
            repairAction);
    }
    return globResult;
}

void MyStrategy::debugUpdate(const PlayerView& playerView, DebugInterface& debugInterface)
{
    debugInterface.send(DebugCommand::Clear());
    debugInterface.getState();
}

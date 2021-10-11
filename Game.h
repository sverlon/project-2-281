// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include <queue>
#include "Round.h"
using namespace std;

struct ZombieLess
{
    bool operator()(const Zombie *one, const Zombie *two)
    {
        if (one->getETA() > two->getETA())
        {
            return true;
        }
        else if (one->getETA() < two->getETA())
        {
            return false;
        }

        // break ties with health
        else if (one->getHealth() > two->getHealth())
        {
            return true;
        }
        else if (one->getHealth() < two->getHealth())
        {
            return false;
        }

        // break ties with name
        else if (one->getName() > two->getName())
        {
            return true;
        }
        return false;
    }
};

struct ZombieActivity
{
    bool operator()(const Zombie *one, const Zombie *two)
    {
        if (one->getRoundsActive() < two->getRoundsActive())
        {
            return true;
        }
        else if (one->getRoundsActive() > two->getRoundsActive())
        {
            return false;
        }
        else if (one->getName() > two->getName())
        {
            return true;
        }
        return false;
    }
};

struct ZombieActivityTwo
{
    bool operator()(const Zombie *one, const Zombie *two)
    {
        if (one->getRoundsActive() > two->getRoundsActive())
        {
            return true;
        }
        else if (one->getRoundsActive() < two->getRoundsActive())
        {
            return false;
        }
        else if (one->getName() > two->getName())
        {
            return true;
        }
        return false;
    }
};

class Game
{
    void getMode(int argc, char **argv);

    void playRound(uint32_t round_number, uint32_t index);

    void printInfo(vector<Zombie> &v, uint32_t i);

    void printInfo(Zombie *z);

    void printStats();

public:
    // defined in .cpp
    // takes command line args
    Game(int argc, char **argv);

    void playGame();

private:
    // the player
    Player player;

    // where we store the zombies that are initially created
    priority_queue<Zombie *, vector<Zombie *>, ZombieLess> zombie_queue;

    // zombies are kept track of in this vector
    vector<Zombie> master_zombies;

    // vector for dead zombies
    vector<Zombie *> dead_zombies;

    // vector for how many zombies to generate in
    // each round
    vector<uint32_t> zombies_per_round;

    // whether stats mode is turned on
    bool stats;

    // number for stats
    uint32_t num_stats;

    // median mode
    bool median;

    // verbose mode
    bool verbose;
};
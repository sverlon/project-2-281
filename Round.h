// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include "Player.h"
using namespace std;

class Round
{
public:
    Round(uint32_t named_zombs, uint32_t rand_zombs)
        : num_named_zombies(named_zombs), num_rand_zombies(rand_zombs)
    {
        named_zombies.reserve(num_named_zombies);
    }

    Round() : num_named_zombies(0), num_rand_zombies(0) {}

    vector<Zombie> &getNamedZombies()
    {
        return named_zombies;
    }

    uint32_t &getNumRandZombs()
    {
        return num_rand_zombies;
    }

private:
    uint32_t num_named_zombies;
    uint32_t num_rand_zombies;
    vector<Zombie> named_zombies;
};
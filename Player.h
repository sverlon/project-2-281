// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include "Zombie.h"
using namespace std;

class Player
{
public:
    // the only constructor needed
    Player() : num_arrows(0), quiver_capacity(0), isDead(false) {}

    void refillArrows()
    {
        num_arrows = quiver_capacity;
    }

    void setQuiverCapacity(uint32_t _quiv_capac)
    {
        quiver_capacity = _quiv_capac;
        refillArrows();
    }

    // if  the player still has arrows,
    // num arrows is decremented and returns
    // true so that the zombie can be hit. Otherwise
    // returnse false.
    bool shoot()
    {
        if (num_arrows == 0)
        {
            return false;
        }
        num_arrows--;
        return true;
    }

    void die()
    {
        isDead = true;
    }

    bool dead()
    {
        return isDead;
    }

    bool hasArrows()
    {
        return num_arrows != 0;
    }

private:
    uint32_t num_arrows;
    uint32_t quiver_capacity;
    bool isDead;
};
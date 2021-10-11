// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include <cstdint>
#include <string>
#include "./P2random.h"
using namespace std;

class Zombie
{
public:
    // randomized zombie
    Zombie()
        : name(P2random::getNextZombieName()),
          distance(static_cast<int32_t>(P2random::getNextZombieDistance())),
          speed(static_cast<int32_t>(P2random::getNextZombieSpeed())),
          health(P2random::getNextZombieHealth()),
          rounds_active(0) {}

    // named zombies
    Zombie(string name_in,
           int32_t distance_in,
           int32_t speed_in,
           uint32_t health_in)
        : name(name_in),
          distance(distance_in),
          speed(speed_in),
          health(health_in),
          rounds_active(0) {}

    // check to see if our player is being eaten
    bool eatingHead()
    {
        if (distance == 0)
        {
            return true;
        }
        return false;
    }

    // get ETA
    int32_t getETA() const
    {
        return distance / speed;
    }

    int32_t getDistance() const
    {
        return distance;
    }

    int32_t getSpeed() const
    {
        return speed;
    }

    uint32_t getHealth() const
    {
        return health;
    }

    // move closer by decrementing ETA
    void moveCloser()
    {
        distance -= speed;
        if (distance < 0)
        {
            distance = 0;
        }
    }

    // check to see if zombie was killed
    bool isDead() const
    {
        if (health == 0)
        {
            return true;
        }
        return false;
    }

    // hit zombie with arrow
    void hit()
    {
        health--;
    }

    // get num rounds active
    uint32_t getRoundsActive() const
    {
        return rounds_active;
    }

    void incrementRoundsActive()
    {
        rounds_active++;
    }

    const string &getName() const
    {
        return name;
    }

private:
    string name;
    int32_t distance;
    int32_t speed;
    uint32_t health;
    uint32_t rounds_active;
};
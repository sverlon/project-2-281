// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include "Game.h"
#include <getopt.h>
#include <iostream>
#include <algorithm>
using namespace std;

// get flags from command line
void Game::getMode(int argc, char **argv)
{
    stats = false;
    median = false;
    verbose = false;

    string num_stats_string;

    int choice;
    int option_index = 0;
    string type;
    option container_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"statistics", required_argument, nullptr, 's'},
        {"median", no_argument, nullptr, 'm'},
        {"verbose", no_argument, nullptr, 'v'},
        {nullptr, 0, nullptr, '\0'}};

    // parse options
    while ((choice = getopt_long(argc, argv, "hs:mv", container_options, &option_index)) != -1)
    {
        switch (choice)
        {
        // help is useless
        case 'h':
            cerr << "nothing to do here\n";
            break;
        // turn stats mode on, get num stats
        case 's':
            stats = true;
            num_stats_string = optarg;
            num_stats = static_cast<uint32_t>(stoi(num_stats_string));
            break;

        // turn median mode on
        case 'm':
            median = true;
            break;

        // turn verbose mode on
        case 'v':
            verbose = true;
            break;
        default:
            cerr << "Invalid output mode specified\n";
            exit(1);
        }
    }
}

Game::Game(int argc, char **argv)
{
    getMode(argc, argv);

    // get rid of first comment
    string junk;
    getline(cin, junk);

    // get quiver capacity, random seed, max distance, max speed, and
    // max health
    uint32_t quiver_capacity;
    uint32_t rand_seed;
    uint32_t max_distance;
    uint32_t max_speed;
    uint32_t max_health;

    // notice how we directly pass a value to
    // the member variable quiver capacity
    cin >> junk >> quiver_capacity;
    cin >> junk >> rand_seed;
    cin >> junk >> max_distance;
    cin >> junk >> max_speed;
    cin >> junk >> max_health;

    player.setQuiverCapacity(quiver_capacity);

    // use inputs to initialize randomization
    P2random::initialize(rand_seed, max_distance, max_speed, max_health);

    // now, we create our zombies vector
    string bar;
    uint32_t sz = 0;
    vector<Round> rounds;
    uint32_t last_round = 0;

    while (cin >> bar)
    {
        string junk;
        uint32_t round_number;
        uint32_t num_rand_zombies;
        uint32_t num_named_zombies;

        cin >> junk >> round_number;
        cin >> junk >> num_rand_zombies;
        cin >> junk >> num_named_zombies;

        sz += num_rand_zombies + num_named_zombies;

        if (last_round + 1 != round_number)
        {
            for (uint32_t i = last_round + 1; i < round_number; i++)
            {
                Round round;
                rounds.push_back(round);
            }
        }

        Round round(num_named_zombies, num_rand_zombies);
        for (uint32_t i = 0; i < num_named_zombies; i++)
        {

            string name;
            int32_t distance;
            int32_t speed;
            uint32_t health;

            cin >> name;
            cin >> junk >> distance;
            cin >> junk >> speed;
            cin >> junk >> health;

            Zombie zombie(name, distance, speed, health);

            round.getNamedZombies().push_back(zombie);
        }
        rounds.push_back(round);
        last_round = round_number;
    }

    zombies_per_round.resize(rounds.size(), 0);

    master_zombies.reserve(sz);

    for (uint32_t i = 0; i < rounds.size(); i++)
    {
        for (uint32_t j = 0; j < rounds[i].getNumRandZombs(); j++)
        {
            Zombie rand;
            master_zombies.push_back(rand);
            zombies_per_round[i]++;
        }
        for (uint32_t j = 0; j < rounds[i].getNamedZombies().size(); j++)
        {
            Zombie named(rounds[i].getNamedZombies()[j]);
            master_zombies.push_back(named);
            zombies_per_round[i]++;
        }
    }
}

void Game::printInfo(vector<Zombie> &v, uint32_t i)
{
    cout << v[i].getName();
    cout << " (distance: " << v[i].getDistance();
    cout << ", speed: " << v[i].getSpeed();
    cout << ", health: " << v[i].getHealth();
    cout << ")\n";
}

void Game::printInfo(Zombie *z)
{
    cout << z->getName();
    cout << " (distance: " << z->getDistance();
    cout << ", speed: " << z->getSpeed();
    cout << ", health: " << z->getHealth();
    cout << ")\n";
}

void Game::playRound(uint32_t round_number, uint32_t index)
{
    if (verbose)
    {
        cout << "Round: " << round_number << "\n";
    }

    player.refillArrows();

    Zombie eater;

    if (zombie_queue.size())
    {
        for (uint32_t i = 0; i < index; i++)
        {
            if (!master_zombies[i].isDead())
            {
                master_zombies[i].moveCloser();
                master_zombies[i].incrementRoundsActive();
                if (verbose)
                {
                    cout << "Moved: ";
                    printInfo(master_zombies, i);
                }
            }

            if (master_zombies[i].eatingHead() && !player.dead())
            {
                eater = master_zombies[i];

                player.die();
            }
        }
    }

    if (!player.dead())
    {
        if (round_number <= zombies_per_round.size())
        {
            for (uint32_t i = index; i < zombies_per_round[round_number - 1] + index; i++)
            {
                zombie_queue.push(&master_zombies[i]);
                master_zombies[i].incrementRoundsActive();
                if (verbose)
                {
                    cout << "Created: ";
                    printInfo(master_zombies, i);
                }
            }
        }

        Zombie *last_killed_zombie = nullptr;

        while (player.hasArrows() && zombie_queue.size())
        {
            Zombie *close_zombie = zombie_queue.top();
            while (!close_zombie->isDead() && player.shoot())
            {
                close_zombie->hit();
            }

            if (close_zombie->isDead())
            {
                zombie_queue.pop();
                dead_zombies.push_back(close_zombie);

                if (verbose)
                {
                    cout << "Destroyed: ";
                    printInfo(close_zombie);
                }

                if (!zombie_queue.size())
                {
                    last_killed_zombie = close_zombie;
                    break;
                }
            }
        }
        if (median && dead_zombies.size())
        {
            cout << "At the end of round ";
            cout << round_number;
            cout << ", the median zombie lifetime is ";

            ZombieActivity a;
            vector<Zombie *> dead_copy(dead_zombies);

            sort(dead_copy.begin(), dead_copy.end(), a);

            size_t mid = dead_copy.size() / 2;

            if (dead_copy.size() % 2 == 0)
            {
                cout << (dead_copy[mid - 1]->getRoundsActive() + dead_copy[mid]->getRoundsActive()) / 2;
            }
            else
            {
                cout << dead_copy[mid]->getRoundsActive();
            }
            cout << "\n";
        }

        if (!zombie_queue.size() &&
            (round_number > zombies_per_round.size() || zombies_per_round[round_number] == 0))
        {
            cout << "VICTORY IN ROUND ";
            cout << round_number;
            cout << "! " << last_killed_zombie->getName();
            cout << " was the last zombie.\n";
        }
    }

    else
    {
        cout << "DEFEAT IN ROUND ";
        cout << round_number << "!";
        cout << " " << eater.getName();
        cout << " ate your brains!\n";
    }
}

void Game::playGame()
{
    uint32_t index = 0;
    uint32_t i = 0;
    for (i = 0; i < zombies_per_round.size() && !player.dead(); i++)
    {
        playRound(i + 1, index);
        index += zombies_per_round[i];
    }

    while (zombie_queue.size() && !player.dead())
    {
        playRound(i + 1, index);
        i++;
    }

    if (stats)
    {
        printStats();
    }
    return;
}

void Game::printStats()
{

    cout << "Zombies still active: ";
    cout << zombie_queue.size();
    cout << "\n";

    cout << "First zombies killed:\n";

    for (uint32_t i = 0; i < num_stats && i < dead_zombies.size(); i++)
    {
        cout << dead_zombies[i]->getName();
        cout << " " << i + 1 << "\n";
    }

    cout << "Last zombies killed:\n";

    uint32_t lowerBound;
    if (num_stats > dead_zombies.size())
    {
        lowerBound = 0;
    }
    else
    {
        lowerBound = (static_cast<uint32_t>(dead_zombies.size()) - num_stats);
    }

    uint32_t i = static_cast<uint32_t>(dead_zombies.size()) - 1;
    uint32_t index = num_stats > dead_zombies.size() ? static_cast<uint32_t>(dead_zombies.size()) : num_stats;
    while (i > 0 && i > lowerBound)
    {
        cout << dead_zombies[i]->getName();
        cout << " " << index << "\n";
        i--;
        index--;
    }
    cout << dead_zombies[i]->getName();
    cout << " " << index << "\n";

    cout << "Most active zombies:\n";

    ZombieActivity z;
    ZombieActivityTwo m;

    while (zombie_queue.size())
    {
        Zombie *a = zombie_queue.top();
        zombie_queue.pop();
        dead_zombies.push_back(a);
    }

    sort(dead_zombies.begin(), dead_zombies.end(), z);

    if (num_stats > dead_zombies.size())
    {
        lowerBound = 0;
    }
    else
    {
        lowerBound = (static_cast<uint32_t>(dead_zombies.size()) - num_stats);
    }

    i = static_cast<uint32_t>(dead_zombies.size()) - 1;
    while (i > 0 && i > lowerBound)
    {
        cout << dead_zombies[i]->getName();
        cout << " " << dead_zombies[i]->getRoundsActive() << "\n";
        i--;
    }
    cout << dead_zombies[i]->getName();
    cout << " " << dead_zombies[i]->getRoundsActive() << "\n";

    cout << "Least active zombies:\n";

    sort(dead_zombies.begin(), dead_zombies.end(), m);

    i = static_cast<uint32_t>(dead_zombies.size()) - 1;
    while (i > 0 && i > lowerBound)
    {
        cout << dead_zombies[i]->getName();
        cout << " " << dead_zombies[i]->getRoundsActive() << "\n";
        i--;
    }
    cout << dead_zombies[i]->getName();
    cout << " " << dead_zombies[i]->getRoundsActive() << "\n";
}
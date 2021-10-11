// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include <iostream>
#include "Game.h"
using namespace std;

int main(int argc, char **argv)
{
    // speed up runtime
    ios_base::sync_with_stdio(false);

    Game niraqToTheBance(argc, argv);

    niraqToTheBance.playGame();

    return 0;
}
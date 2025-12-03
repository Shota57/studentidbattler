#include <cstdlib>
#include <ctime>
#include "game.h"

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    runGame();
    return 0;
}


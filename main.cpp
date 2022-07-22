//memory leak detection
/*#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
*/
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "SDL_setup.h"
#include "Game.h"
#include "ConfigFile.h"
#include <fstream>

#undef main

int main( int argc, char* argv[] )
{
    //TODO: Needs error handling
    gConfig_file = new ConfigFile("config/game.cfg");
    if (!init_graphics())
    {
        printf("Failed to initialize Graphics!\n");
    }
    else
    {
        //initialize randomness
        srand(static_cast<unsigned>(time(nullptr)));

        Game game;

        game.start_game();
    }
    
    SDL_Delay(1000);
    return 0;
}

#include "BattleshipGame.h"
#include <iostream>

int main(int argc, char** argv) {
    BattleshipGame game;
    
    if (!game.Initialize()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return -1;
    }
    
    std::cout << "Starting Battleships game..." << std::endl;
    
    game.Run();
    
    std::cout << "Game ended. Thank you for playing!" << std::endl;
    return 0;
}
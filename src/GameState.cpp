#include "GameState.h"

GameState::GameState() {
    Reset();
}

void GameState::Reset() {
    currentState = GameStateType::ShipPlacement;
    gameEnded = false;
    victoryMessage = "";
    isPlayerTurn = true;
    playerShipsRemaining = 0;
    aiShipsRemaining = 0;
}


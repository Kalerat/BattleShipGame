#pragma once
#include <string>

enum class GameStateType {
    ShipPlacement,
    Battle,
    GameOver
};

enum class CellState {
    Empty,
    Ship,
    Hit,
    Miss,
    Preview,
    InvalidPreview
};

struct GridPosition {
    int x, y;
    
    GridPosition(int x = 0, int y = 0) : x(x), y(y) {}
    
    bool operator==(const GridPosition& other) const {
        return x == other.x && y == other.y;
    }
};

class GameState {
public:
    GameState();
    
    void Reset();
    GameStateType GetState() const { return currentState; }
    void SetState(GameStateType newState) { currentState = newState; }

    bool IsGameEnded() const { return gameEnded; }
    void SetGameEnded(bool ended) { gameEnded = ended; }
    
    const std::string& GetVictoryMessage() const { return victoryMessage; }
    void SetVictoryMessage(const std::string& message) { victoryMessage = message; }
    
    bool IsPlayerTurn() const { return isPlayerTurn; }
    void SetPlayerTurn(bool playerTurn) { isPlayerTurn = playerTurn; }
    
    int GetPlayerShipsRemaining() const { return playerShipsRemaining; }
    void SetPlayerShipsRemaining(int count) { playerShipsRemaining = count; }
    
    int GetAIShipsRemaining() const { return aiShipsRemaining; }
    void SetAIShipsRemaining(int count) { aiShipsRemaining = count; }

private:
    GameStateType currentState;
    bool gameEnded;
    std::string victoryMessage;
    bool isPlayerTurn;
    int playerShipsRemaining;
    int aiShipsRemaining;
};

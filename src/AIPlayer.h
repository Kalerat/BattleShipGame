#pragma once
#include <vector>
#include <random>
#include "GameState.h"
#include "Ship.h"

class Grid;

class AIPlayer {
public:
    AIPlayer();
    
    void Reset();
    void InitializeAI();
    
    void PlaceShips(Grid& aiGrid);
    GridPosition GetTarget(const Grid& playerGrid);
    
    void SetLastHit(GridPosition hit) { lastHit = hit; }
    void ClearLastHit() { lastHit = GridPosition(-1, -1); }
    void ClearTargetQueue() { targetQueue.clear(); }
    
    ShipManager& GetShipManager() { return shipManager; }
    const ShipManager& GetShipManager() const { return shipManager; }

private:
    ShipManager shipManager;
    GridPosition lastHit;
    std::vector<GridPosition> targetQueue;
    std::mt19937 randomGenerator;
    
    bool IsValidShipPlacement(const Grid& grid, int startX, int startY, int shipSize, bool horizontal) const;
    void PlaceShip(Grid& grid, int startX, int startY, int shipSize, bool horizontal) const;
};

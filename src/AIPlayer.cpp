#include "AIPlayer.h"
#include "Grid.h"
#include <iostream>

AIPlayer::AIPlayer() : lastHit(-1, -1) {
    std::random_device rd;
    randomGenerator.seed(rd());
    InitializeAI();
}

void AIPlayer::Reset() {
    lastHit = GridPosition(-1, -1);
    targetQueue.clear();
    shipManager.Reset();
}

void AIPlayer::InitializeAI() {
    shipManager.GetShips().clear();
    // 1x Battleship (Size 5)
    shipManager.GetShips().emplace_back(ShipType::Battleship, 5, "AI Battleship");
    
    // 2x Cruiser (Size 4)
    shipManager.GetShips().emplace_back(ShipType::Cruiser, 4, "AI Cruiser 1");
    shipManager.GetShips().emplace_back(ShipType::Cruiser, 4, "AI Cruiser 2");
    
    // 3x Destroyer (Size 3)
    shipManager.GetShips().emplace_back(ShipType::Destroyer, 3, "AI Destroyer 1");
    shipManager.GetShips().emplace_back(ShipType::Destroyer, 3, "AI Destroyer 2");
    shipManager.GetShips().emplace_back(ShipType::Destroyer, 3, "AI Destroyer 3");
    
    // 4x Submarine (Size 2)
    shipManager.GetShips().emplace_back(ShipType::Submarine, 2, "AI Submarine 1");
    shipManager.GetShips().emplace_back(ShipType::Submarine, 2, "AI Submarine 2");
    shipManager.GetShips().emplace_back(ShipType::Submarine, 2, "AI Submarine 3");
    shipManager.GetShips().emplace_back(ShipType::Submarine, 2, "AI Submarine 4");
    
    targetQueue.clear();
    lastHit = GridPosition(-1, -1);
}

void AIPlayer::PlaceShips(Grid& aiGrid) {
    std::cout << "AI is placing ships..." << std::endl;
    
    std::uniform_int_distribution<> posDist(0, GRID_SIZE - 1);
    std::uniform_int_distribution<> orientDist(0, 1);
    
    for (auto& ship : shipManager.GetShips()) {
        bool placed = false;
        int attempts = 0;
        
        while (!placed && attempts < 1000) {
            int x = posDist(randomGenerator);
            int y = posDist(randomGenerator);
            bool horizontal = orientDist(randomGenerator) == 0;
            
            if (IsValidShipPlacement(aiGrid, x, y, ship.size, horizontal)) {
                PlaceShip(aiGrid, x, y, ship.size, horizontal);
                ship.placed = true;
                placed = true;
                //std::cout << "AI placed " << ship.name << " at " << (char)('A' + x) << (y + 1) 
                //         << " (" << (horizontal ? "horizontal" : "vertical") << ")" << std::endl;
            }
            attempts++;
        }
    }
}


// TODO: Implement a more sophisticated AI targeting strategy
// For now, this will use a simple random targeting strategy with some basic logic for hits
GridPosition AIPlayer::GetTarget(const Grid& playerGrid) {
    // If we have targets in the queue (from previous hits), use them first
    while (!targetQueue.empty()) {
        GridPosition target = targetQueue.back();
        targetQueue.pop_back();
        
        // Check if this target is still valid (not already targeted)
        CellState cell = playerGrid.GetCell(target.x, target.y);
        if (cell != CellState::Hit && cell != CellState::Miss) {
            return target;
        }
    }
    
    // If we hit something last turn, target adjacent cells
    if (lastHit.x >= 0 && lastHit.y >= 0) {
        std::vector<GridPosition> adjacentCells = {
            {lastHit.x - 1, lastHit.y},
            {lastHit.x + 1, lastHit.y},
            {lastHit.x, lastHit.y - 1},
            {lastHit.x, lastHit.y + 1}
        };
        
        // Add valid adjacent cells to target queue
        for (auto& cell : adjacentCells) {
            if (playerGrid.IsValidPosition(cell.x, cell.y)) {
                CellState cellState = playerGrid.GetCell(cell.x, cell.y);
                if (cellState != CellState::Hit && cellState != CellState::Miss) {
                    targetQueue.push_back(cell);
                }
            }
        }
        
        // Clear last hit since we've processed it
        lastHit = GridPosition(-1, -1);
        
        // If we have targets, use the first one
        if (!targetQueue.empty()) {
            GridPosition target = targetQueue.back();
            targetQueue.pop_back();
            return target;
        }
    }
    
    // Random targeting
    std::uniform_int_distribution<> dist(0, GRID_SIZE - 1);
    GridPosition target;
    int attempts = 0;
    
    do {
        target.x = dist(randomGenerator);
        target.y = dist(randomGenerator);
        attempts++;
        
        // Safety check to prevent infinite loop
        if (attempts > 1000) {
            // Find any untargeted cell
            for (int y = 0; y < GRID_SIZE; ++y) {
                for (int x = 0; x < GRID_SIZE; ++x) {
                    CellState cell = playerGrid.GetCell(x, y);
                    if (cell != CellState::Hit && cell != CellState::Miss) {
                        return GridPosition(x, y);
                    }
                }
            }
            // If no valid targets found, return default (should not happen)
            return GridPosition(0, 0);
        }
    } while (playerGrid.GetCell(target.x, target.y) == CellState::Hit || 
             playerGrid.GetCell(target.x, target.y) == CellState::Miss);
    
    return target;
}

bool AIPlayer::IsValidShipPlacement(const Grid& grid, int startX, int startY, int shipSize, bool horizontal) const {
    return shipManager.IsValidPlacement(grid, startX, startY, shipSize, horizontal);
}

void AIPlayer::PlaceShip(Grid& grid, int startX, int startY, int shipSize, bool horizontal) const {
    shipManager.PlaceShip(grid, startX, startY, shipSize, horizontal);
}

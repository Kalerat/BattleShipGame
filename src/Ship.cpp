#include "Ship.h"
#include "Grid.h"
#include <vector>

ShipManager::ShipManager() : currentShipIndex(0), isHorizontal(true) {
    InitializeShips();
}

void ShipManager::Reset() {
    currentShipIndex = 0;
    isHorizontal = true;
    for (auto& ship : ships) {
        ship.placed = false;
    }
}

void ShipManager::InitializeShips() {
    ships.clear();
    // 1x Battleship (Size 5)
    ships.emplace_back(ShipType::Battleship, 5, "Battleship");
    
    // 2x Cruiser (Size 4)
    ships.emplace_back(ShipType::Cruiser, 4, "Cruiser 1");
    ships.emplace_back(ShipType::Cruiser, 4, "Cruiser 2");
    
    // 3x Destroyer (Size 3)
    ships.emplace_back(ShipType::Destroyer, 3, "Destroyer 1");
    ships.emplace_back(ShipType::Destroyer, 3, "Destroyer 2");
    ships.emplace_back(ShipType::Destroyer, 3, "Destroyer 3");
    
    // 4x Submarine (Size 2)
    ships.emplace_back(ShipType::Submarine, 2, "Submarine 1");
    ships.emplace_back(ShipType::Submarine, 2, "Submarine 2");
    ships.emplace_back(ShipType::Submarine, 2, "Submarine 3");
    ships.emplace_back(ShipType::Submarine, 2, "Submarine 4");
}

bool ShipManager::IsValidPlacement(const Grid& grid, int startX, int startY, int shipSize, bool horizontal) const {
    // Check bounds
    if (horizontal) {
        if (startX + shipSize > GRID_SIZE || startY >= GRID_SIZE) {
            return false;
        }
    } else {
        if (startX >= GRID_SIZE || startY + shipSize > GRID_SIZE) {
            return false;
        }
    }
    
    // Check if cells are empty and not adjacent to other ships
    for (int i = 0; i < shipSize; ++i) {
        int checkX = horizontal ? startX + i : startX;
        int checkY = horizontal ? startY : startY + i;
        
        // Check if current cell is occupied
        CellState currentCell = grid.GetCell(checkX, checkY);
        if (currentCell != CellState::Empty && 
            currentCell != CellState::Preview && 
            currentCell != CellState::InvalidPreview) {
            return false;
        }
        
        // Check adjacent cells (no touching ships rule)
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                
                int adjX = checkX + dx;
                int adjY = checkY + dy;
                
                if (grid.IsValidPosition(adjX, adjY)) {
                    if (grid.GetCell(adjX, adjY) == CellState::Ship) {
                        return false;
                    }
                }
            }
        }
    }
    
    return true;
}

void ShipManager::PlaceShip(Grid& grid, int startX, int startY, int shipSize, bool horizontal) const {
    for (int i = 0; i < shipSize; ++i) {
        int x = horizontal ? startX + i : startX;
        int y = horizontal ? startY : startY + i;
        grid.SetCell(x, y, CellState::Ship);
    }
}

bool ShipManager::IsShipSunk(const Grid& grid, GridPosition hit) const {
    // Find all connected ship cells and check if they're all hit
    std::vector<std::vector<bool>> visited(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
    std::vector<GridPosition> shipCells;
    std::vector<GridPosition> toCheck;
    
    // Start flood fill from hit position
    toCheck.push_back(hit);
    
    while (!toCheck.empty()) {
        GridPosition current = toCheck.back();
        toCheck.pop_back();
        
        if (current.x < 0 || current.x >= GRID_SIZE || current.y < 0 || current.y >= GRID_SIZE || 
            visited[current.y][current.x]) {
            continue;
        }
        
        CellState cell = grid.GetCell(current.x, current.y);
        if (cell == CellState::Ship || cell == CellState::Hit) {
            visited[current.y][current.x] = true;
            shipCells.push_back(current);
            
            // Add adjacent cells to check
            toCheck.push_back({current.x - 1, current.y});
            toCheck.push_back({current.x + 1, current.y});
            toCheck.push_back({current.x, current.y - 1});
            toCheck.push_back({current.x, current.y + 1});
        }
    }
    
    // Check if all ship cells are hit
    for (auto& cell : shipCells) {
        if (grid.GetCell(cell.x, cell.y) != CellState::Hit) {
            return false;
        }
    }
    
    return true;
}

bool ShipManager::AllShipsPlaced() const {
    return currentShipIndex >= ships.size();
}

int ShipManager::GetRemainingShipCount(ShipType type) const {
    int count = 0;
    for (const auto& ship : ships) {
        if (ship.type == type && !ship.placed) {
            count++;
        }
    }
    return count;
}

int ShipManager::GetTotalShipCount(ShipType type) const {
    int count = 0;
    for (const auto& ship : ships) {
        if (ship.type == type) {
            count++;
        }
    }
    return count;
}

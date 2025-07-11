#pragma once
#include <vector>
#include <string>
#include "GameState.h"

class Grid;

enum class ShipType {
    Battleship = 0,
    Cruiser = 1,
    Destroyer = 2,
    Submarine = 3,
};

struct Ship {
    ShipType type;
    int size;
    std::string name;
    bool placed;
    
    Ship(ShipType t, int s, const std::string& n) 
        : type(t), size(s), name(n), placed(false) {}
};

class ShipManager {
public:
    ShipManager();
    
    void Reset();
    void InitializeShips();
    
    const std::vector<Ship>& GetShips() const { return ships; }
    std::vector<Ship>& GetShips() { return ships; }
    
    bool IsValidPlacement(const Grid& grid, int startX, int startY, int shipSize, bool horizontal) const;
    void PlaceShip(Grid& grid, int startX, int startY, int shipSize, bool horizontal) const;
    
    bool IsShipSunk(const Grid& grid, GridPosition hit) const;
    
    bool AllShipsPlaced() const;
    int GetCurrentShipIndex() const { return currentShipIndex; }
    void SetCurrentShipIndex(int index) { currentShipIndex = index; }
    void MoveToNextShip() { currentShipIndex++; }
    
    bool IsHorizontal() const { return isHorizontal; }
    void SetHorizontal(bool horizontal) { isHorizontal = horizontal; }
    void ToggleOrientation() { isHorizontal = !isHorizontal; }
    
    // Helper methods for ship count display
    int GetRemainingShipCount(ShipType type) const;
    int GetTotalShipCount(ShipType type) const;

private:
    std::vector<Ship> ships;
    int currentShipIndex;
    bool isHorizontal;
};

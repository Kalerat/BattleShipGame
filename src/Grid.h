#pragma once
#include <array>
#include <string>
#include <SDL3/SDL.h>
#include "GameState.h"

class Renderer;

constexpr int GRID_SIZE = 10;
constexpr int CELL_SIZE = 30;

class Grid {
public:
    Grid();
    
    void Clear();
    void Reset();
    
    CellState GetCell(int x, int y) const;
    void SetCell(int x, int y, CellState state);
    
    bool IsValidPosition(int x, int y) const;
    
    void ClearPreview();
    
    void Render(Renderer* renderer, int offsetX, int offsetY, const std::string& title, bool isPlayerGrid = false) const;
    
    int CountRemainingShips() const;
    
    const std::array<std::array<CellState, GRID_SIZE>, GRID_SIZE>& GetGrid() const { return grid; }

private:
    std::array<std::array<CellState, GRID_SIZE>, GRID_SIZE> grid;
};

#include "Grid.h"
#include "Renderer.h"

Grid::Grid() {
    Clear();
}

void Grid::Clear() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = CellState::Empty;
        }
    }
}

void Grid::Reset() {
    Clear();
}

CellState Grid::GetCell(int x, int y) const {
    if (!IsValidPosition(x, y)) {
        return CellState::Empty;
    }
    return grid[y][x];
}

void Grid::SetCell(int x, int y, CellState state) {
    if (IsValidPosition(x, y)) {
        grid[y][x] = state;
    }
}

bool Grid::IsValidPosition(int x, int y) const {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

void Grid::ClearPreview() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == CellState::Preview || grid[i][j] == CellState::InvalidPreview) {
                grid[i][j] = CellState::Empty;
            }
        }
    }
}

void Grid::Render(Renderer* renderer, int offsetX, int offsetY, const std::string& title, bool isPlayerGrid) const {
    renderer->RenderGrid(offsetX, offsetY, grid, title, isPlayerGrid);
}

int Grid::CountRemainingShips() const {
    int count = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == CellState::Ship) {
                count++;
            }
        }
    }
    return count;
}

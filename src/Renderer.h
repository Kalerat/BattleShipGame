#pragma once
#include <string>
#include <array>
#include <SDL3/SDL.h>
#include "GameState.h"

class ShipManager;

class Renderer {
public:
    Renderer(SDL_Renderer* sdlRenderer);
    
    void RenderGrid(int offsetX, int offsetY, const std::array<std::array<CellState, 10>, 10>& grid, 
                   const std::string& title, bool isPlayerGrid = false) const;
    
    void RenderText(const std::string& text, int x, int y) const;
    void RenderTextLarge(const std::string& text, int x, int y, int scale) const;
    
    void RenderShipPlacementUI(const ShipManager& shipManager, int instructionY, int listX, int listY) const;
    void RenderGameOverUI(const std::string& victoryMessage, SDL_FRect& playAgainButton, bool playAgainButtonHovered) const;
    
    SDL_Color GetCellColor(CellState state, bool isPlayerGrid) const;

private:
    SDL_Renderer* renderer;
    
    void RenderGridLabels(int offsetX, int offsetY) const;
    void RenderChar(char c, int x, int y) const;
    void RenderCharLarge(char c, int x, int y, int scale) const;
    void InitializeFontArray(std::array<std::array<uint8_t, 8>, 256>& font) const;
    
    // Colors
    SDL_Color backgroundColor = {30, 30, 30, 255};
    SDL_Color emptyCellColor = {60, 60, 60, 255};
    SDL_Color shipCellColor = {0, 100, 0, 255};
    SDL_Color hitCellColor = {200, 50, 50, 255};
    SDL_Color missCellColor = {50, 100, 200, 255};
    SDL_Color gridLineColor = {150, 150, 150, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color previewValidColor = {50, 200, 50, 128};
    SDL_Color previewInvalidColor = {200, 50, 50, 128};
};

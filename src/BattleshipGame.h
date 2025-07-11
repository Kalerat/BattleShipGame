#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include "GameState.h"
#include "Grid.h"
#include "Ship.h"
#include "AIPlayer.h"
#include "Renderer.h"

class BattleshipGame {
public:
    BattleshipGame();
    ~BattleshipGame();
    
    bool Initialize();
    void Run();
    void Cleanup();

private:
    // Core components
    std::unique_ptr<GameState> gameState;
    std::unique_ptr<Grid> playerGrid;
    std::unique_ptr<Grid> targetGrid;
    std::unique_ptr<Grid> aiGrid;
    std::unique_ptr<ShipManager> shipManager;
    std::unique_ptr<AIPlayer> aiPlayer;
    std::unique_ptr<Renderer> renderer;
    
    // SDL components
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    
    // Game loop
    bool isRunning;
    
    // UI state
    GridPosition mouseGridPos;
    SDL_FRect playAgainButton;
    bool playAgainButtonHovered;
    
    // Game flow methods
    void HandleEvents();
    void Update();
    void Render();
    void RestartGame();
    
    // Event handlers
    void HandleShipPlacementClick(int mouseX, int mouseY);
    void HandleShipPlacementKeyboard(SDL_Keycode key);
    void HandleGridClick(int mouseX, int mouseY);
    
    // Ship placement
    void UpdateShipPreview(int mouseX, int mouseY);
    void UpdateShipPreviewAtCurrentPosition();
    
    // Combat
    void ProcessPlayerShot(GridPosition target);
    void ProcessAIShot(GridPosition target);
    void ProcessAITurn();
    
    // Game logic
    void CheckVictoryCondition();
    GridPosition ScreenToGrid(int mouseX, int mouseY, bool isPlayerGrid);
    
    // AI turn timing
    int aiTurnDelay;
    
    // Constants
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int GRID_MARGIN = 50;
    static constexpr int GRID_SPACING = 50;
};

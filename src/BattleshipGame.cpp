#include "BattleshipGame.h"
#include <iostream>

BattleshipGame::BattleshipGame() 
    : window(nullptr), sdlRenderer(nullptr), isRunning(false),
      mouseGridPos(-1, -1), playAgainButton{0, 0, 0, 0}, playAgainButtonHovered(false),
      aiTurnDelay(0) {
    
    // Initialize components
    gameState = std::make_unique<GameState>();
    playerGrid = std::make_unique<Grid>();
    targetGrid = std::make_unique<Grid>();
    aiGrid = std::make_unique<Grid>();
    shipManager = std::make_unique<ShipManager>();
    aiPlayer = std::make_unique<AIPlayer>();
}

BattleshipGame::~BattleshipGame() {
    Cleanup();
}

bool BattleshipGame::Initialize() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL3 failed to initialize: " << SDL_GetError() << std::endl;
        return false;
    }
    
    window = SDL_CreateWindow("Battleships - Player vs Computer", 
                             WINDOW_WIDTH, WINDOW_HEIGHT, 
                             SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }
    
    sdlRenderer = SDL_CreateRenderer(window, nullptr);
    if (!sdlRenderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize renderer component
    renderer = std::make_unique<Renderer>(sdlRenderer);
    
    // Show initial ship preview
    UpdateShipPreviewAtCurrentPosition();
    
    isRunning = true;
    return true;
}

void BattleshipGame::Run() {
    while (isRunning) {
        HandleEvents();
        Update();
        Render();
    }
}

void BattleshipGame::Cleanup() {
    if (sdlRenderer) {
        SDL_DestroyRenderer(sdlRenderer);
        sdlRenderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void BattleshipGame::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (gameState->GetState() == GameStateType::ShipPlacement) {
                        HandleShipPlacementClick(event.button.x, event.button.y);
                    } else if (gameState->GetState() == GameStateType::Battle) {
                        HandleGridClick(event.button.x, event.button.y);
                    } else if (gameState->GetState() == GameStateType::GameOver) {
                        // Check if play again button was clicked
                        if (event.button.x >= playAgainButton.x && 
                            event.button.x <= playAgainButton.x + playAgainButton.w &&
                            event.button.y >= playAgainButton.y && 
                            event.button.y <= playAgainButton.y + playAgainButton.h) {
                            std::cout << "Play Again button clicked!" << std::endl;
                            RestartGame();
                        }
                    }
                }
                break;
            case SDL_EVENT_MOUSE_MOTION:
                if (gameState->GetState() == GameStateType::ShipPlacement) {
                    UpdateShipPreview(event.motion.x, event.motion.y);
                } else if (gameState->GetState() == GameStateType::GameOver) {
                    // Check if mouse is over play again button
                    playAgainButtonHovered = (event.motion.x >= playAgainButton.x && 
                                            event.motion.x <= playAgainButton.x + playAgainButton.w &&
                                            event.motion.y >= playAgainButton.y && 
                                            event.motion.y <= playAgainButton.y + playAgainButton.h);
                }
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE) {
                    isRunning = false;
                } else if (gameState->GetState() == GameStateType::ShipPlacement) {
                    HandleShipPlacementKeyboard(event.key.key);
                } else if (gameState->GetState() == GameStateType::GameOver && event.key.key == SDLK_SPACE) {
                    RestartGame();
                }
                break;
        }
    }
}

void BattleshipGame::Update() {
    // Handle AI turn
    if (gameState->GetState() == GameStateType::Battle && 
        !gameState->IsPlayerTurn() && !gameState->IsGameEnded()) {
        ProcessAITurn();
    }
}

void BattleshipGame::Render() {
    // Clear screen
    SDL_SetRenderDrawColor(sdlRenderer, 30, 30, 30, 255);
    SDL_RenderClear(sdlRenderer);
    
    // Calculate grid positions
    int playerGridX = GRID_MARGIN;
    int playerGridY = GRID_MARGIN + 30;
    int targetGridX = GRID_MARGIN * 2 + GRID_SIZE * CELL_SIZE + GRID_SPACING;
    int targetGridY = GRID_MARGIN + 30;
    
    // Render grids
    playerGrid->Render(renderer.get(), playerGridX, playerGridY, "Your Ships", true);
    
    if (gameState->GetState() == GameStateType::Battle) {
        targetGrid->Render(renderer.get(), targetGridX, targetGridY, "Target Grid");
    }
    
    // Render UI elements
    if (gameState->GetState() == GameStateType::ShipPlacement) {
        int instructionY = GRID_MARGIN + GRID_SIZE * CELL_SIZE + 50;
        int listX = GRID_MARGIN * 2 + GRID_SIZE * CELL_SIZE + GRID_SPACING;
        int listY = GRID_MARGIN + 50;
        
        renderer->RenderShipPlacementUI(*shipManager, instructionY, listX, listY);
    }
    
    if (gameState->GetState() == GameStateType::GameOver) {
        renderer->RenderGameOverUI(gameState->GetVictoryMessage(), playAgainButton, playAgainButtonHovered);
    }
    
    SDL_RenderPresent(sdlRenderer);
}

void BattleshipGame::HandleShipPlacementClick(int mouseX, int mouseY) {
    // Check if mouse is over player grid
    int playerGridX = GRID_MARGIN;
    int playerGridY = GRID_MARGIN + 30;
    
    if (mouseX >= playerGridX && mouseX < playerGridX + GRID_SIZE * CELL_SIZE &&
        mouseY >= playerGridY && mouseY < playerGridY + GRID_SIZE * CELL_SIZE &&
        !shipManager->AllShipsPlaced()) {
        
        GridPosition pos = ScreenToGrid(mouseX, mouseY, true);
        const auto& ships = shipManager->GetShips();
        int currentShipIndex = shipManager->GetCurrentShipIndex();
        
        // Check if placement is valid
        if (shipManager->IsValidPlacement(*playerGrid, pos.x, pos.y, 
                                        ships[currentShipIndex].size, shipManager->IsHorizontal())) {
            // Clear preview first
            playerGrid->ClearPreview();
            
            // Place the ship
            shipManager->PlaceShip(*playerGrid, pos.x, pos.y, 
                                 ships[currentShipIndex].size, shipManager->IsHorizontal());
            shipManager->GetShips()[currentShipIndex].placed = true;
            
            // Move to next ship
            shipManager->MoveToNextShip();
            
            // Check if all ships are placed
            if (shipManager->AllShipsPlaced()) {
                gameState->SetState(GameStateType::Battle);
                aiPlayer->PlaceShips(*aiGrid);
                gameState->SetPlayerShipsRemaining(playerGrid->CountRemainingShips());
                gameState->SetAIShipsRemaining(aiGrid->CountRemainingShips());
                std::cout << "All ships placed! Starting battle phase..." << std::endl;
                std::cout << "Your turn! Click on the right grid to fire." << std::endl;
            } else {
                const auto& currentShip = shipManager->GetShips()[shipManager->GetCurrentShipIndex()];
                std::cout << "Ship placed! Now place your " << currentShip.name 
                         << " (" << currentShip.size << " cells)" << std::endl;
            }
        }
    }
}

void BattleshipGame::HandleShipPlacementKeyboard(SDL_Keycode key) {
    switch (key) {
        case SDLK_R:
        case SDLK_SPACE:
            // Rotate ship
            shipManager->ToggleOrientation();
            std::cout << "Ship orientation: " << (shipManager->IsHorizontal() ? "Horizontal" : "Vertical") << std::endl;
            UpdateShipPreviewAtCurrentPosition();
            break;
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_5:
        case SDLK_6:
        case SDLK_7:
        case SDLK_8:
        case SDLK_9:
        case SDLK_0:
            // Select ship by number
            int shipIndex = key - SDLK_1;
            const auto& ships = shipManager->GetShips();
            if (shipIndex < ships.size() && !ships[shipIndex].placed) {
                shipManager->SetCurrentShipIndex(shipIndex);
                std::cout << "Selected " << ships[shipIndex].name 
                         << " (" << ships[shipIndex].size << " cells)" << std::endl;
                UpdateShipPreviewAtCurrentPosition();
            }
            break;
    }
}

void BattleshipGame::HandleGridClick(int mouseX, int mouseY) {
    // Only process clicks if it's player's turn and game hasn't ended
    if (!gameState->IsPlayerTurn() || gameState->IsGameEnded()) return;
    
    // Check if click is in target grid area
    int targetGridX = GRID_MARGIN * 2 + GRID_SIZE * CELL_SIZE + GRID_SPACING;
    int targetGridY = GRID_MARGIN + 30;
    
    if (mouseX >= targetGridX && mouseX < targetGridX + GRID_SIZE * CELL_SIZE &&
        mouseY >= targetGridY && mouseY < targetGridY + GRID_SIZE * CELL_SIZE) {
        
        GridPosition pos = ScreenToGrid(mouseX, mouseY, false);
        
        // Only allow firing at cells that haven't been targeted yet
        if (targetGrid->GetCell(pos.x, pos.y) == CellState::Empty) {
            ProcessPlayerShot(pos);
        }
    }
}

void BattleshipGame::UpdateShipPreview(int mouseX, int mouseY) {
    // Clear previous preview
    playerGrid->ClearPreview();
    
    // Check if mouse is over player grid
    int playerGridX = GRID_MARGIN;
    int playerGridY = GRID_MARGIN + 30;
    
    if (mouseX >= playerGridX && mouseX < playerGridX + GRID_SIZE * CELL_SIZE &&
        mouseY >= playerGridY && mouseY < playerGridY + GRID_SIZE * CELL_SIZE &&
        !shipManager->AllShipsPlaced()) {
        
        mouseGridPos = ScreenToGrid(mouseX, mouseY, true);
        const auto& ships = shipManager->GetShips();
        int currentShipIndex = shipManager->GetCurrentShipIndex();
        
        // Check if placement is valid
        bool isValid = shipManager->IsValidPlacement(*playerGrid, mouseGridPos.x, mouseGridPos.y, 
                                                   ships[currentShipIndex].size, shipManager->IsHorizontal());
        
        // Draw preview
        for (int i = 0; i < ships[currentShipIndex].size; ++i) {
            int x = shipManager->IsHorizontal() ? mouseGridPos.x + i : mouseGridPos.x;
            int y = shipManager->IsHorizontal() ? mouseGridPos.y : mouseGridPos.y + i;
            
            if (playerGrid->IsValidPosition(x, y)) {
                if (playerGrid->GetCell(x, y) == CellState::Empty) {
                    playerGrid->SetCell(x, y, isValid ? CellState::Preview : CellState::InvalidPreview);
                }
            }
        }
    }
}

void BattleshipGame::UpdateShipPreviewAtCurrentPosition() {
    // Clear previous preview
    playerGrid->ClearPreview();
    
    // Check if we have a valid current position and are in ship placement mode
    if (!shipManager->AllShipsPlaced()) {
        // If we don't have a valid mouse position yet, use a default position (center of grid)
        if (mouseGridPos.x < 0 || mouseGridPos.x >= GRID_SIZE || 
            mouseGridPos.y < 0 || mouseGridPos.y >= GRID_SIZE) {
            mouseGridPos.x = GRID_SIZE / 2;
            mouseGridPos.y = GRID_SIZE / 2;
        }
        
        const auto& ships = shipManager->GetShips();
        int currentShipIndex = shipManager->GetCurrentShipIndex();
        
        // Check if placement is valid
        bool isValid = shipManager->IsValidPlacement(*playerGrid, mouseGridPos.x, mouseGridPos.y, 
                                                   ships[currentShipIndex].size, shipManager->IsHorizontal());
        
        // Draw preview at current position
        for (int i = 0; i < ships[currentShipIndex].size; ++i) {
            int x = shipManager->IsHorizontal() ? mouseGridPos.x + i : mouseGridPos.x;
            int y = shipManager->IsHorizontal() ? mouseGridPos.y : mouseGridPos.y + i;
            
            if (playerGrid->IsValidPosition(x, y)) {
                if (playerGrid->GetCell(x, y) == CellState::Empty) {
                    playerGrid->SetCell(x, y, isValid ? CellState::Preview : CellState::InvalidPreview);
                }
            }
        }
    }
}

void BattleshipGame::ProcessPlayerShot(GridPosition target) {
    // Check if hit or miss
    if (aiGrid->GetCell(target.x, target.y) == CellState::Ship) {
        // Update both grids
        aiGrid->SetCell(target.x, target.y, CellState::Hit);
        targetGrid->SetCell(target.x, target.y, CellState::Hit);
        std::cout << "HIT at " << (char)('A' + target.x) << (target.y + 1) << "!" << std::endl;
        
        // Check if ship is sunk
        if (shipManager->IsShipSunk(*aiGrid, target)) {
            std::cout << "You sunk an enemy ship!" << std::endl;
        }
    } else {
        targetGrid->SetCell(target.x, target.y, CellState::Miss);
        std::cout << "MISS at " << (char)('A' + target.x) << (target.y + 1) << std::endl;
    }
    
    // Switch to AI turn
    gameState->SetPlayerTurn(false);
    std::cout << "AI's turn..." << std::endl;
    
    // Check victory condition
    CheckVictoryCondition();
}

void BattleshipGame::ProcessAIShot(GridPosition target) {
    std::cout << "AI fires at " << (char)('A' + target.x) << (target.y + 1) << std::endl;
    
    // Check if hit or miss
    if (playerGrid->GetCell(target.x, target.y) == CellState::Ship) {
        playerGrid->SetCell(target.x, target.y, CellState::Hit);
        std::cout << "AI HIT your ship!" << std::endl;
        
        // Remember this hit for next turn
        aiPlayer->SetLastHit(target);
        
        // Check if ship is sunk
        if (shipManager->IsShipSunk(*playerGrid, target)) {
            std::cout << "AI sunk one of your ships!" << std::endl;
            aiPlayer->ClearLastHit();
            aiPlayer->ClearTargetQueue();
        }
    } else {
        playerGrid->SetCell(target.x, target.y, CellState::Miss);
        std::cout << "AI missed." << std::endl;
    }
    
    // Switch to player turn
    gameState->SetPlayerTurn(true);
    std::cout << "Your turn!" << std::endl;
    
    // Check victory condition
    CheckVictoryCondition();
}

void BattleshipGame::ProcessAITurn() {
    aiTurnDelay++;
    
    // Add a small delay to make AI moves visible
    if (aiTurnDelay < 30) return; // ~0.5 second delay at 60 FPS
    aiTurnDelay = 0;
    
    GridPosition target = aiPlayer->GetTarget(*playerGrid);
    ProcessAIShot(target);
}

void BattleshipGame::CheckVictoryCondition() {
    gameState->SetPlayerShipsRemaining(playerGrid->CountRemainingShips());
    gameState->SetAIShipsRemaining(aiGrid->CountRemainingShips());
    
    std::cout << "Ships remaining - Player: " << gameState->GetPlayerShipsRemaining() 
              << ", AI: " << gameState->GetAIShipsRemaining() << std::endl;
    
    if (gameState->GetPlayerShipsRemaining() == 0) {
        gameState->SetVictoryMessage("GAME OVER - AI WINS!");
        gameState->SetGameEnded(true);
        gameState->SetState(GameStateType::GameOver);
        std::cout << "AI wins! All your ships have been sunk." << std::endl;
        std::cout << "Press SPACE to restart." << std::endl;
    } else if (gameState->GetAIShipsRemaining() == 0) {
        gameState->SetVictoryMessage("VICTORY - YOU WIN!");
        gameState->SetGameEnded(true);
        gameState->SetState(GameStateType::GameOver);
        std::cout << "You win! All enemy ships have been sunk." << std::endl;
        std::cout << "Press SPACE to restart." << std::endl;
    }
}

void BattleshipGame::RestartGame() {
    std::cout << "RestartGame function called!" << std::endl;
    
    // Reset all components
    gameState->Reset();
    playerGrid->Reset();
    targetGrid->Reset();
    aiGrid->Reset();
    shipManager->Reset();
    aiPlayer->Reset();
    
    // Reset UI state
    mouseGridPos = GridPosition(-1, -1);
    playAgainButtonHovered = false;
    aiTurnDelay = 0;
    
    // Show initial preview
    UpdateShipPreviewAtCurrentPosition();
    
    std::cout << "Game restarted! Place your ships." << std::endl;
}

GridPosition BattleshipGame::ScreenToGrid(int mouseX, int mouseY, bool isPlayerGrid) {
    int offsetX = isPlayerGrid ? GRID_MARGIN : (GRID_MARGIN * 2 + GRID_SIZE * CELL_SIZE + GRID_SPACING);
    int offsetY = GRID_MARGIN + 30;
    
    int gridX = (mouseX - offsetX) / CELL_SIZE;
    int gridY = (mouseY - offsetY) / CELL_SIZE;
    
    // Clamp to grid bounds
    gridX = std::max(0, std::min(gridX, GRID_SIZE - 1));
    gridY = std::max(0, std::min(gridY, GRID_SIZE - 1));
    
    return GridPosition(gridX, gridY);
}

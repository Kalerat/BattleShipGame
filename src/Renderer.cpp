#include "Renderer.h"
#include "Ship.h"
#include <array>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int GRID_MARGIN = 50;
constexpr int GRID_SPACING = 50;
constexpr int CELL_SIZE = 30;

Renderer::Renderer(SDL_Renderer* sdlRenderer) : renderer(sdlRenderer) {
}

void Renderer::RenderGrid(int offsetX, int offsetY, const std::array<std::array<CellState, 10>, 10>& grid, 
                         const std::string& title, bool isPlayerGrid) const {
    // Render title
    RenderText(title, offsetX + (10 * CELL_SIZE) / 2 - 50, offsetY - 25);
    
    // Render grid cells
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            SDL_FRect cellRect = {
                (float)(offsetX + col * CELL_SIZE),
                (float)(offsetY + row * CELL_SIZE),
                (float)CELL_SIZE,
                (float)CELL_SIZE
            };
            
            // Fill cell with appropriate color
            SDL_Color cellColor = GetCellColor(grid[row][col], isPlayerGrid);
            SDL_SetRenderDrawColor(renderer, cellColor.r, cellColor.g, cellColor.b, cellColor.a);
            SDL_RenderFillRect(renderer, &cellRect);
            
            // Draw cell border
            SDL_SetRenderDrawColor(renderer, gridLineColor.r, gridLineColor.g, gridLineColor.b, gridLineColor.a);
            SDL_RenderRect(renderer, &cellRect);
        }
    }
    
    // Render grid labels
    RenderGridLabels(offsetX, offsetY);
}

void Renderer::RenderGridLabels(int offsetX, int offsetY) const {
    // Column labels (A-J)
    for (int col = 0; col < 10; ++col) {
        std::string label = std::string(1, 'A' + col);
        RenderText(label, offsetX + col * CELL_SIZE + CELL_SIZE / 2 - 5, offsetY - 15);
    }
    
    // Row labels (1-10)
    for (int row = 0; row < 10; ++row) {
        std::string label = std::to_string(row + 1);
        RenderText(label, offsetX - 20, offsetY + row * CELL_SIZE + CELL_SIZE / 2 - 5);
    }
}

void Renderer::RenderText(const std::string& text, int x, int y) const {
    for (size_t i = 0; i < text.length(); ++i) {
        RenderChar(text[i], x + (int)i * 8, y);
    }
}

void Renderer::RenderTextLarge(const std::string& text, int x, int y, int scale) const {
    for (size_t i = 0; i < text.length(); ++i) {
        RenderCharLarge(text[i], x + (int)i * 8 * scale, y, scale);
    }
}

void Renderer::InitializeFontArray(std::array<std::array<uint8_t, 8>, 256>& font) const {
     for (auto& charData : font) {
            charData.fill(0);
        }
        
        // Numbers 0-9
        font[48] = {0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}; // 0
        font[49] = {0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00}; // 1
        font[50] = {0x3C, 0x66, 0x06, 0x0C, 0x18, 0x30, 0x7E, 0x00}; // 2
        font[51] = {0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0x00}; // 3
        font[52] = {0x0C, 0x1C, 0x3C, 0x6C, 0x7E, 0x0C, 0x0C, 0x00}; // 4
        font[53] = {0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00}; // 5
        font[54] = {0x3C, 0x66, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00}; // 6
        font[55] = {0x7E, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x18, 0x00}; // 7
        font[56] = {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00}; // 8
        font[57] = {0x3C, 0x66, 0x66, 0x3E, 0x06, 0x66, 0x3C, 0x00}; // 9
        
        // Letters A-Z
        font[65] = {0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00}; // A
        font[66] = {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00}; // B
        font[67] = {0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00}; // C
        font[68] = {0x78, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0x78, 0x00}; // D
        font[69] = {0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x7E, 0x00}; // E
        font[70] = {0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00}; // F
        font[71] = {0x3C, 0x66, 0x60, 0x6E, 0x66, 0x66, 0x3C, 0x00}; // G
        font[72] = {0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00}; // H
        font[73] = {0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00}; // I
        font[74] = {0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x6C, 0x38, 0x00}; // J
        font[75] = {0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66, 0x00}; // K
        font[76] = {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00}; // L
        font[77] = {0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x00}; // M
        font[78] = {0x66, 0x76, 0x7E, 0x6E, 0x66, 0x66, 0x66, 0x00}; // N
        font[79] = {0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}; // O
        font[80] = {0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x00}; // P
        font[81] = {0x3C, 0x66, 0x66, 0x66, 0x6A, 0x6C, 0x36, 0x00}; // Q
        font[82] = {0x7C, 0x66, 0x66, 0x7C, 0x78, 0x6C, 0x66, 0x00}; // R
        font[83] = {0x3C, 0x66, 0x60, 0x3C, 0x06, 0x66, 0x3C, 0x00}; // S
        font[84] = {0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}; // T
        font[85] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}; // U
        font[86] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00}; // V
        font[87] = {0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00}; // W
        font[88] = {0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00}; // X
        font[89] = {0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00}; // Y
        font[90] = {0x7E, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00}; // Z
        
        // Space
        font[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Space
        // Exclamation mark
        font[33] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00}; // !
}

void Renderer::RenderChar(char c, int x, int y) const {
    // Simple bitmap font - 8x8 pixels per character
    static std::array<std::array<uint8_t, 8>, 256> font;
    static bool fontInitialized = false;
    
    if (!fontInitialized) {
        InitializeFontArray(font);
        fontInitialized = true;
    }
    
    // Convert character to uppercase if it's lowercase
    if (c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
    }
    
    // Draw the character bitmap
    SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
    
    for (int row = 0; row < 8; ++row) {
        uint8_t line = font[(uint8_t)c][row];
        for (int col = 0; col < 8; ++col) {
            if (line & (0x80 >> col)) {
                SDL_FRect pixelRect = {(float)(x + col), (float)(y + row), 1.0f, 1.0f};
                SDL_RenderFillRect(renderer, &pixelRect);
            }
        }
    }
}

void Renderer::RenderCharLarge(char c, int x, int y, int scale) const {
    // Simple bitmap font - 8x8 pixels per character, scaled up
    static std::array<std::array<uint8_t, 8>, 256> font;
    static bool fontInitialized = false;
    
    if (!fontInitialized) {
        InitializeFontArray(font);
        fontInitialized = true;
    }
    
    // Convert character to uppercase if it's lowercase
    if (c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
    }
    
    // Draw the character bitmap scaled up
    SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
    
    for (int row = 0; row < 8; ++row) {
        uint8_t line = font[(uint8_t)c][row];
        for (int col = 0; col < 8; ++col) {
            if (line & (0x80 >> col)) {
                // Draw a scaled pixel (scale x scale rectangle)
                SDL_FRect pixelRect = {(float)(x + col * scale), (float)(y + row * scale), (float)scale, (float)scale};
                SDL_RenderFillRect(renderer, &pixelRect);
            }
        }
    }
}

SDL_Color Renderer::GetCellColor(CellState state, bool isPlayerGrid) const {
    switch (state) {
        case CellState::Empty:
            return emptyCellColor;
        case CellState::Ship:
            return isPlayerGrid ? shipCellColor : emptyCellColor; // Hide enemy ships
        case CellState::Hit:
            return hitCellColor;
        case CellState::Miss:
            return missCellColor;
        case CellState::Preview:
            return previewValidColor;
        case CellState::InvalidPreview:
            return previewInvalidColor;
        default:
            return emptyCellColor;
    }
}

void Renderer::RenderShipPlacementUI(const ShipManager& shipManager, int instructionY, int listX, int listY) const {
    const auto& ships = shipManager.GetShips();
    int currentShipIndex = shipManager.GetCurrentShipIndex();
    
    if (currentShipIndex < ships.size()) {
        std::string currentShip = "Place: " + ships[currentShipIndex].name + " (" + std::to_string(ships[currentShipIndex].size) + " cells)";
        RenderText(currentShip, GRID_MARGIN, instructionY);
        
        std::string orientation = "Orientation: " + std::string(shipManager.IsHorizontal() ? "Horizontal" : "Vertical");
        RenderText(orientation, GRID_MARGIN, instructionY + 15);
        
        RenderText("R/Space: Rotate", GRID_MARGIN, instructionY + 30);
        RenderText("1-0: Select ship", GRID_MARGIN, instructionY + 45);
    }
    
    RenderText("Ships to Place:", listX, listY);
    
    for (size_t i = 0; i < ships.size(); ++i) {
        std::string status = ships[i].placed ? "✓" : " ";
        // Format ship number with right alignment (pad single digits with space)
        std::string shipNumber = (i + 1 < 10) ? " " + std::to_string(i + 1) : std::to_string(i + 1);
        std::string shipInfo = shipNumber + ". " + status + " " + ships[i].name;
        
        // Highlight current ship
        if (i == currentShipIndex && !ships[i].placed) {
            // Draw background highlight
            SDL_FRect highlight = {(float)listX - 2, (float)(listY + 15 + i * 15 - 2), 200.0f, 14.0f};
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 128);
            SDL_RenderFillRect(renderer, &highlight);
        }
        
        RenderText(shipInfo, listX, listY + 15 + i * 15);
    }
}

void Renderer::RenderGameOverUI(const std::string& victoryMessage, SDL_FRect& playAgainButton, bool playAgainButtonHovered) const {
    // Draw semi-transparent overlay
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_FRect overlay = {0, 0, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);
    
    // Calculate text size and position for Game over message
    int textScale = 4; // Make text 4x bigger
    int messageWidth = victoryMessage.length() * 8 * textScale;
    int messageHeight = 8 * textScale;
    int messageX = (WINDOW_WIDTH - messageWidth) / 2;
    int messageY = WINDOW_HEIGHT / 2 - 60;
    
    // Draw background for Game over message
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_FRect messageBg = {(float)messageX - 20, (float)messageY - 15, 
                          (float)messageWidth + 40, (float)messageHeight + 30};
    SDL_RenderFillRect(renderer, &messageBg);
    // Draw border for Game over message
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderRect(renderer, &messageBg);

    // Render large Game over message
    RenderTextLarge(victoryMessage, messageX, messageY, textScale);
    
    // Setup play again button
    std::string buttonText = "PLAY AGAIN!";
    int buttonTextScale = 2;
    int buttonWidth = buttonText.length() * 8 * buttonTextScale + 40;
    int buttonHeight = 8 * buttonTextScale + 20;
    int buttonX = (WINDOW_WIDTH - buttonWidth) / 2;
    int buttonY = messageY + messageHeight + 40;
    
    // Store button bounds for click detection
    playAgainButton = {(float)buttonX, (float)buttonY, (float)buttonWidth, (float)buttonHeight};
    
    // Draw button background
    if (playAgainButtonHovered) {
        SDL_SetRenderDrawColor(renderer, 80, 150, 80, 255); // Lighter green when hovered
    } else {
        SDL_SetRenderDrawColor(renderer, 60, 120, 60, 255); // Dark green
    }
    SDL_RenderFillRect(renderer, &playAgainButton);
    
    // Draw button border
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderRect(renderer, &playAgainButton);
    
    // Draw button text
    int buttonTextX = buttonX + (buttonWidth - buttonText.length() * 8 * buttonTextScale) / 2;
    int buttonTextY = buttonY + (buttonHeight - 8 * buttonTextScale) / 2;
    RenderTextLarge(buttonText, buttonTextX, buttonTextY, buttonTextScale);
    
    // Draw additional instruction text
    std::string instructionText = "Or press SPACE";
    int instructionX = (WINDOW_WIDTH - instructionText.length() * 8) / 2;
    int instructionY = buttonY + buttonHeight + 20;
    
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 200);
    SDL_FRect instructionBg = {(float)instructionX - 10, (float)instructionY - 5, 
                              (float)instructionText.length() * 8 + 20, 18};
    SDL_RenderFillRect(renderer, &instructionBg);
    
    RenderText(instructionText, instructionX, instructionY);
}

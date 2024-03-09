#include "raylib.h"
#include <vector>
#include <cstddef>

// Initialize window dimensions
const int screenWidth = 400;
const int screenHeight = 800;
const int blockSize = 25; // Initialize the size of each block in the grid
int halfScreenWidth = screenWidth / 2;
int halfScreenHeight = screenHeight / 2;
float moveSpeed = 0.1f; // Adjust this value to control the movement speed
float fallSpeed = 0.5f; // Adjust this value to control the falling speed
const int gridWidth = screenWidth / blockSize;
const int gridHeight = screenHeight / blockSize;
int score = 0; // Initialize the score to 0
std::vector<std::vector<int>> grid(gridWidth, std::vector<int>(gridHeight, 0));  // 0 represents an empty cell

struct Tetromino {
    int x, y;  // Position of the Tetromino in the grid
    Color color;
    std::vector<std::vector<int>> shape;
    int rotation;  // Rotation state of the Tetromino (0, 1, 2, 3 for 0, 90, 180, 270 degrees)
};

Tetromino currentTetromino;

enum TetrominoType {
    I_SHAPE,
    O_SHAPE,
    T_SHAPE,
    L_SHAPE,
    S_SHAPE,
    Z_SHAPE,
    J_SHAPE

};

// Function to initialize a tetromino with a specific type and position
void InitializeTetromino(Tetromino& tetromino, TetrominoType type) {
    // Initialize Tetromino properties (color, shape, etc.)
    tetromino.color = GOLD; // Change current tetromino color
    tetromino.rotation = 0;  // Initial rotation state

    switch (type) {
        case I_SHAPE:
            tetromino.shape = {
                {1, 1, 1, 1}  // I shape
            };
            break;
        case O_SHAPE:
            tetromino.shape = {
                {1, 1},
                {1, 1}  // O shape
            };
            break;
        case T_SHAPE:
            tetromino.shape = {
                {0, 1, 0},
                {1, 1, 1}  // T shape
            };
            break;
        case L_SHAPE:
            tetromino.shape = {
                {1, 0},
                {1, 0},
                {1, 1}  // L shape
            };
            break;
        case S_SHAPE:
            tetromino.shape = {
                {0, 1, 1},
                {1, 1, 0}   // S shape
            };
            break;
        case Z_SHAPE:
            tetromino.shape = {
                {1, 1, 0},
                {0, 1, 1}   // Z shape
            };
            break;
        case J_SHAPE:
            tetromino.shape = {
                {0, 1},
                {0, 1},
                {1, 1}  // J shape
            };
            break;
        // Add more cases for other shapes as needed
    }

    tetromino.x = gridWidth / 2 - tetromino.shape[0].size() / 2;
    tetromino.y = 0;
}

bool CanMoveDown(const Tetromino& tetromino) {
    // Check if the Tetromino can move down by examining the grid
    for (size_t i = 0; i < tetromino.shape.size(); ++i) {
        for (size_t j = 0; j < tetromino.shape[i].size(); ++j) {
            if (tetromino.shape[i][j] == 1) {
                int gridX = tetromino.x + j;
                int gridY = tetromino.y + i + 1;

                // Check if the next position is within bounds and not occupied
                if (gridX < 0 || gridX >= gridWidth || gridY >= gridHeight || (gridY >= 0 && grid[gridX][gridY] != 0)) {
                    return false;  // Collision detected
                }
            }
        }
    }

    return true;  // No collision, can move down
}

// Function to move the tetromino down in the grid
void MoveTetrominoDown(Tetromino& tetromino) {
    static float fallCounter = 0.0f;

    fallCounter += GetFrameTime();


    // Adjust the fallSpeed value to control the falling speed
    if (fallCounter >= fallSpeed) {
        // Move the Tetromino down in the grid
        if (CanMoveDown(tetromino)) {
            tetromino.y++;
        } else {
            // Lock the Tetromino in place and update the grid
            for (size_t i = 0; i < tetromino.shape.size(); ++i) {
                for (size_t j = 0; j < tetromino.shape[i].size(); ++j) {
                    if (tetromino.shape[i][j] == 1 && tetromino.y + i >= 0) {
                        grid[tetromino.x + j][tetromino.y + i] = 1;
                    }
                }
            }

            // Spawn a new Tetromino
            TetrominoType randomType = static_cast<TetrominoType>(GetRandomValue(0, 6));
            InitializeTetromino(currentTetromino, randomType);
        }

        fallCounter = 0.0f;
    }
}

bool CanMoveLeft(const Tetromino& tetromino) {
    // Check if the Tetromino can move left by examining the grid
    for (size_t i = 0; i < tetromino.shape.size(); ++i) {
        for (size_t j = 0; j < tetromino.shape[i].size(); ++j) {
            if (tetromino.shape[i][j] == 1) {
                int gridX = tetromino.x + j - 1;
                int gridY = tetromino.y + i;

                // Check if the next position is within bounds and not occupied
                if (gridX < 0 || (gridY >= 0 && grid[gridX][gridY] != 0)) {
                    return false;  // Collision detected
                }
            }
        }
    }

    return true;  // No collision, can move left
}

bool CanMoveRight(const Tetromino& tetromino) {
    // Check if the Tetromino can move right by examining the grid
    for (size_t i = 0; i < tetromino.shape.size(); ++i) {
        for (size_t j = 0; j < tetromino.shape[i].size(); ++j) {
            if (tetromino.shape[i][j] == 1) {
                int gridX = tetromino.x + j + 1;
                int gridY = tetromino.y + i;

                // Check if the next position is within bounds and not occupied
                if (gridX >= gridWidth || (gridY >= 0 && grid[gridX][gridY] != 0)) {
                    return false;  // Collision detected
                }
            }
        }
    }

    return true;  // No collision, can move right
}

void MoveTetrominoLeft(Tetromino& tetromino) {
    static float moveLeftCounter = 0.0f;

    moveLeftCounter += GetFrameTime();

    // Adjust the moveSpeed value to control the movement speed
    if (moveLeftCounter >= moveSpeed) {
        // Move the Tetromino to the left in the grid
        if (CanMoveLeft(tetromino)) {
            tetromino.x--;
        }

        moveLeftCounter = 0.0f;
    }
}

void MoveTetrominoRight(Tetromino& tetromino) {
    static float moveRightCounter = 0.0f;

    moveRightCounter += GetFrameTime();

    // Adjust the moveSpeed value to control the movement speed
    if (moveRightCounter >= moveSpeed) {
        // Move the Tetromino to the right in the grid
        if (CanMoveRight(tetromino)) {
            tetromino.x++;
        }

        moveRightCounter = 0.0f;
    }
}

// Function to check if a Tetromino can be rotated without collision
bool CanRotate(const Tetromino& tetromino) {
    // Check for collisions after rotation
    for (size_t i = 0; i < tetromino.shape.size(); ++i) {
        for (size_t j = 0; j < tetromino.shape[i].size(); ++j) {
            if (tetromino.shape[i][j] == 1) {
                int gridX = tetromino.x + j;
                int gridY = tetromino.y + i;

                // Check if the next position is within bounds and not occupied
                if (gridX < 0 || gridX >= gridWidth || gridY < 0 || gridY >= gridHeight || grid[gridX][gridY] != 0) {
                    return false;  // Collision detected
                }
            }
        }
    }

    return true;  // No collision, can rotate
}

// Function to rotate current Tetromino clockwise
void RotateTetrominoClockwise(Tetromino& tetromino) {
    // Save the current shape for later comparison
    std::vector<std::vector<int>> originalShape = tetromino.shape;

    // Rotate the shape clockwise by resizing the vector
    tetromino.shape.clear();
    for (size_t i = 0; i < originalShape[0].size(); ++i) {
        std::vector<int> newRow;
        for (int j = static_cast<int>(originalShape.size()) - 1; j >= 0; --j) {
            newRow.push_back(originalShape[j][i]);
        }
        tetromino.shape.push_back(newRow);
    }

    // Increment the rotation state (mod 4 to keep it within 0-3)
    tetromino.rotation = (tetromino.rotation + 1) % 4;

    // Check for collisions and revert if necessary
    if (!CanRotate(tetromino)) {
        tetromino.shape = originalShape;
        tetromino.rotation = (tetromino.rotation - 1 + 4) % 4;  // Revert rotation state
    }
}

void DrawGrid() {
    // Draw the grid with alternating colors for each cell
    for (size_t i = 0; i < gridWidth; ++i) {
        for (size_t j = 0; j < gridHeight; ++j) {
            Rectangle cell = { static_cast<float>(i * blockSize), static_cast<float>(j * blockSize), static_cast<float>(blockSize), static_cast<float>(blockSize) };
            Color color = (i + j) % 2 == 0 ? DARKGRAY : BLACK; // Change grid colors
            DrawRectangle(cell.x, cell.y, cell.width, cell.height, color);
        }
    }
}

void DrawTetromino(const Tetromino& tetromino) {
    // Draw the Tetromino
    for (size_t i = 0; i < tetromino.shape.size(); ++i) {
        for (size_t j = 0; j < tetromino.shape[i].size(); ++j) {
            if (tetromino.shape[i][j] == 1) {
                Rectangle block = {
                    static_cast<float>((tetromino.x + j) * blockSize),
                    static_cast<float>((tetromino.y + i) * blockSize),
                    static_cast<float>(blockSize),
                    static_cast<float>(blockSize)
                };
                DrawRectangle(block.x, block.y, block.width, block.height, tetromino.color);
            }
        }
    }
}

void DrawLandedBlocks() {
    // Draw landed blocks
    for (size_t i = 0; i < gridWidth; ++i) {
        for (size_t j = 0; j < gridHeight; ++j) {
            if (grid[i][j] == 1) {
                Rectangle block = {
                    static_cast<float>(i * blockSize),
                    static_cast<float>(j * blockSize),
                    static_cast<float>(blockSize),
                    static_cast<float>(blockSize)
                };
                DrawRectangle(block.x, block.y, block.width, block.height, LIME); // Change landed tetromino color
            }
        }
    }
}

bool IsRowFull(int row) {
    for (int col = 0; col < gridWidth; ++col) {
        if (grid[col][row] == 0) {
            return false;  // The row is not full
        }
    }
    return true;  // The row is full
}

void ClearRow(int row) {
    // Shift all rows above the cleared row/s down
    for (int col = 0; col < gridWidth; ++col) {
        for (int r = row; r > 0; --r) {
            grid[col][r] = grid[col][r - 1];
        }
        grid[col][0] = 0;  // Set the top row to empty
    }

    // Increase the score
    score += 100;
}

// Clear the grid for a new game
void ClearGrid() {
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            grid[i][j] = 0;
        }
    }
}

// Enum defining the different game states
enum GameState {
    GAME_INIT,   // Initial screen showing game controls
    GAME_PLAY,   // Game in progress
    GAME_PAUSE,  // Game paused
    GAME_OVER    // Game over screen
};

int main() {
    // Initialize game components here
    InitWindow(screenWidth, screenHeight, "Tetris Game");

    SetTargetFPS(60);    

    GameState gameState = GAME_INIT; // Initialize game state to initial screen showing game controls

    // Initialize the first tetromino with random type
    TetrominoType randomType = static_cast<TetrominoType>(GetRandomValue(0, 6));
    InitializeTetromino(currentTetromino, randomType);

    bool windowFocused = true;  // Initialize to true outside the loop

    // Main game loop
    while (!WindowShouldClose()) {
        // Update the window focus status
        windowFocused = IsWindowFocused();

        BeginDrawing();

        DrawGrid();
        DrawLandedBlocks();

        // Display the "Press P to pause" message
        DrawText("Press P to pause the game", halfScreenWidth - MeasureText("Press P to pause the game", 20) / 2, 10, 20, WHITE);

        // Draw score
        DrawText(TextFormat("Score: %d", score), 10, screenHeight - 30, 20, WHITE);

        // Pause the game if the window is not focused
        if (!windowFocused && gameState == GAME_PLAY) {
            gameState = GAME_PAUSE;
        }

        switch (gameState) {
            case GAME_INIT:
                // Implement logic for the initial screen here
                // Display the controls screen
                DrawText("Tetris Controls", halfScreenWidth - MeasureText("Tetris Controls", 40) / 2, halfScreenHeight - 120, 40, WHITE);
                DrawText("To Move The Tetromino Use:", halfScreenWidth - MeasureText("To Move The Tetromino Use:", 20) / 2, halfScreenHeight - 55, 20, WHITE);
                DrawText("Left Arrow - Left", halfScreenWidth - MeasureText("Left Arrow - Left", 20) / 2, halfScreenHeight - 25, 20, WHITE);
                DrawText("Right Arrow - Right", halfScreenWidth - MeasureText("Right Arrow - Right", 20) / 2, halfScreenHeight + 5, 20, WHITE);
                DrawText("Use SPACE to Rotate", halfScreenWidth - MeasureText("Use SPACE to Rotate", 20) / 2, halfScreenHeight + 35, 20, WHITE);
                DrawText("Press SPACE to Start", halfScreenWidth - MeasureText("Press SPACE to Start", 25) / 2, halfScreenHeight + 85, 25, WHITE);

                if (IsKeyPressed(KEY_SPACE)) {
                    gameState = GAME_PLAY; // Switch from initial screen to playing the game
                }
            break;

            case GAME_PLAY:
                // Game loop
                // Input handling, drawing, and other game logic go here

                // Pause the game pressing P
                if (IsKeyPressed(KEY_P)) {
                    gameState = GAME_PAUSE;
                }

                if (IsKeyDown(KEY_DOWN)) {
                    fallSpeed = 0.01f;  // Set a higher fall speed when the down arrow is pressed
                } else {
                    fallSpeed = 0.5f;  // Reset the fall speed to normal when the down arrow is not pressed
                }

                DrawTetromino(currentTetromino);

                MoveTetrominoDown(currentTetromino);

                if (IsKeyDown(KEY_LEFT)) {
                    MoveTetrominoLeft(currentTetromino);
                }

                if (IsKeyDown(KEY_RIGHT)) {
                    MoveTetrominoRight(currentTetromino);
                }

                if (IsKeyPressed(KEY_SPACE)) {
                RotateTetrominoClockwise(currentTetromino);
                }

                // Check and clear full rows
                for (int row = 0; row < gridHeight; ++row) {
                    if (IsRowFull(row)) {
                        ClearRow(row);
                        // Increases the score for every row cleared
                    }

                }

                // Check if the game is over
                for (size_t i = 0; i < gridWidth; ++i) {
                    if (grid[i][0] != 0) {
                        gameState = GAME_OVER;
                    }
                }
            break;

            case GAME_PAUSE:
                // Handle game logic while paused.
                // Display a "Game Paused" message on the screen.
                DrawText("Game Paused", halfScreenWidth - MeasureText("Game Paused", 40) / 2, halfScreenHeight - 60, 40, WHITE);
                DrawText("Press P To Resume", halfScreenWidth - MeasureText("Press P To Resume", 20) / 2, halfScreenHeight -10, 20, WHITE);

                if (IsKeyPressed(KEY_P)) {
                gameState = GAME_PLAY; // Resume the paused game
                }
            break;


            case GAME_OVER:
                // Logic for the game over screen here
                DrawText("Game Over", halfScreenWidth - MeasureText("Game Over", 40) / 2, halfScreenHeight - 70, 40, WHITE);
                DrawText("Press R To Restart", halfScreenWidth - MeasureText("Press R To Restart", 20) / 2, halfScreenHeight -10, 20, WHITE);

                // Calculate the width of the final score text
                const char* finalScoreText = TextFormat("Final Score: %d", score);
                int finalScoreTextWidth = MeasureText(finalScoreText, 30);

                // Draw the final score on the game over screen
                DrawText(finalScoreText, halfScreenWidth - finalScoreTextWidth / 2, halfScreenHeight + 40, 30, WHITE);

                if (IsKeyPressed(KEY_R)) {
                    ClearGrid(); // Clear the grid for a new game
                    gameState = GAME_PLAY; // Switch from game over screen to playing new game
                }
            break;

        }

    EndDrawing();
    }

    // Cleanup resources and close the game window
    CloseWindow();

    return 0;

}

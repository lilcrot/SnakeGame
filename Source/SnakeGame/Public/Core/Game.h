// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Grid;
class Snake;

class Game
{
public:
    Game(const FSettings& SettingsIn);

    TSharedPtr<Grid> GetGrid() const;

    TSharedPtr<Snake> GetSnake() const;

    void Update(float DeltaSeconds, const FInput& Input);


private:
    const FSettings Settings;
    TSharedPtr<Grid> GameGrid;
    TSharedPtr<Snake> GameSnake;

    float MoveSeconds = 0.0f;
    bool bIsGameOver = false;

    void Move(const FInput& Input);
    void UpdateGrid();
    bool CanUpdateTime(float DeltaSeconds);
    bool IsDied() const;
};

}  // namespace SnakeGame

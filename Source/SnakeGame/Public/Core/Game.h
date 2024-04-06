// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Grid;
class Snake;
class Food;

class Game
{
public:
    Game(const FSettings& SettingsIn);

    TSharedPtr<Grid> GetGrid() const;
    TSharedPtr<Snake> GetSnake() const;
    TSharedPtr<Food> GetFood() const;

    void Update(float DeltaSeconds, const FInput& Input);

private:
    const FSettings Settings;

    TSharedPtr<Grid> GameGrid;
    TSharedPtr<Snake> GameSnake;
    TSharedPtr<Food> GameFood;

    float MoveSeconds = 0.0f;
    bool bIsGameOver = false;
    uint32 Score = 0;


    void UpdateGrid();
    bool CanUpdateTime(float DeltaSeconds);
    bool IsDied() const;

    void GenerateFood();
    bool IsFoodTaken() const;
};

}  // namespace SnakeGame

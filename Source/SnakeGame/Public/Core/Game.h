// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "Utils.h"

namespace SnakeGame
{
class Grid;
class Snake;
class Food;

class Game
{
public:
    Game(const FSettings& SettingsIn, const IPositionRandomizerPtr& randomizer = MakeShared<PositionRandomizer>());

    TSharedPtr<Grid> GetGrid() const;
    TSharedPtr<Snake> GetSnake() const;
    TSharedPtr<Food> GetFood() const;

    void Update(float DeltaSeconds, const FInput& Input);

    uint32 GetScore() const;
    float GetGameTime() const;

    void SubscribeOnGameplayEvent(const GameplayEventCallback Callback);

private:
    const FSettings Settings;

    TSharedPtr<Grid> GameGrid;
    TSharedPtr<Snake> GameSnake;
    TSharedPtr<Food> GameFood;

    float GameTime = 0.0f;
    float MoveSeconds = 0.0f;
    bool bIsGameOver = false;
    uint32 Score = 0;

    TArray<GameplayEventCallback> GameEventCallbacks;

    void UpdateGrid();
    bool CanUpdateTime(float DeltaSeconds);
    bool IsDied() const;

    void GenerateFood();
    bool IsFoodTaken() const;

    void DispatchEvent(const EGameplayEvent& Event);
};

}  // namespace SnakeGame

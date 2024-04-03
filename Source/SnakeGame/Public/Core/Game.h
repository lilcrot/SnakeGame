// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Grid;

class Game
{
public:
    Game(const FSettings& SettingsIn);

    TSharedPtr<Grid> GetGameGrid() const;

private:
    const FSettings Settings;
    TSharedPtr<Grid> GameGrid;
};

}  // namespace SnakeGame

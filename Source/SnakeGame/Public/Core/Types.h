// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"

namespace SnakeGame
{
struct FDimension
{
    uint32 Width = 0;
    uint32 Height = 0;
};

enum class ECellGridType
{
    Empty = 0,
    Wall = 1
};
struct FSettings
{
    FDimension GridDimension;
};
}  // namespace SnakeGame
// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"

namespace SnakeGame
{
struct FDimension
{
    int32 Width = 0;
    int32 Height = 0;
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
// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Grid
{
public:
    // It's implied that InDim does NOT include walls
    Grid(const FDimension& InDim);

    FDimension GetDimension() const;

private:
    const FDimension Dim;
    TArray<ECellGridType> Cells;

    void InitWalls();
    void PrintDebug();

    int32 PosToCellsIndex(const int32 x, const int32 y) const;
};
}  // namespace SnakeGame

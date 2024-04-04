// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Grid
{
public:
    // @param InDim: it's implied that does NOT include walls
    Grid(const FDimension& InDim);

    FDimension GetDimension() const;

private:
    const FDimension Dim;
    TArray<ECellGridType> Cells;

    void InitWalls();

    uint32 PosToCellsIndex(const uint32 x, const uint32 y) const;
};
}  // namespace SnakeGame

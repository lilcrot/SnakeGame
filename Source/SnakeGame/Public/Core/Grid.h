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

    void Update(const TPositionPtr* Links, ECellGridType CellType);
    bool HitTest(const FPosition& Position, ECellGridType CellType) const;

    /**
     * Returns center position in grid (walls included)
     * @return Position center of grid
     */
    static FPosition GetCenter(const uint32 Width, const uint32 Height);

private:
    const FDimension Dim;
    TArray<ECellGridType> Cells;

    TMap<ECellGridType, TArray<uint32>> IndexesByTypeMap = {
        {ECellGridType::Snake, {}},  //
        {ECellGridType::Wall, {}},   //
    };

    void InitWalls();
    void FreeCellsByType(ECellGridType CellType);

    uint32 PosToCellsIndex(const uint32 x, const uint32 y) const;
    uint32 PosToCellsIndex(const FPosition& Position) const;
};
}  // namespace SnakeGame

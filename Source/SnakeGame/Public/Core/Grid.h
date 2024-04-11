// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "Utils.h"

namespace SnakeGame
{
class Grid
{
public:
    // @param InDim: it's implied that does NOT include walls
    Grid(const FDimension& InDim, const IPositionRandomizerPtr& Randomizer = MakeShared<PositionRandomizer>());

    /**
     * Returns center position in grid (walls included)
     * @return Position center of grid
     */
    static FPosition GetCenter(const uint32 Width, const uint32 Height);

    FDimension GetDimension() const;

    void Update(const TPositionPtr* Links, const ECellGridType& CellType);
    void Update(const FPosition& Position, const ECellGridType& CellType);

    bool HitTest(const FPosition& Position, const ECellGridType& CellType) const;

    /**
     * Returns empty position by ref
     * @param[out]  position on the grid
     * @return bool  true if empty position exists (if position doesn't exist then snake body fills whole grid)
     */
    UE_NODISCARD bool GetRandomEmptyPosition(FPosition& Position) const;

private:
    TSharedPtr<IPositionRandomizer> GridPositionRandomizer;

    const FDimension Dim;
    TArray<ECellGridType> Cells;

    TMap<ECellGridType, TArray<uint32>> IndexesByTypeMap = {
        {ECellGridType::Snake, {}},  //
        {ECellGridType::Wall, {}},   //
        {ECellGridType::Food, {}}    //
    };

    void InitWalls();

    void FreeCellsByType(const ECellGridType& CellType);
    void UpdateInternal(const FPosition& Position, const ECellGridType& CellType);

    uint32 PosToIndex(const uint32 x, const uint32 y) const;
    uint32 PosToIndex(const FPosition& Position) const;
};
}  // namespace SnakeGame

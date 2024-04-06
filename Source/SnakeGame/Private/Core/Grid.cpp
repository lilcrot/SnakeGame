// Educational project Snake Game
#include "Core/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

using namespace SnakeGame;

namespace
{
// Width: Walls at the top and bottom
// Height: Walls on the left and right side
constexpr int32 NumWallsInLine = 2;

}  // namespace

Grid::Grid(const FDimension& InDim)  //
    : Dim(FDimension{InDim.Width + NumWallsInLine, InDim.Height + NumWallsInLine})
{
    /* Dimension ( 4 * 3 without walls )
     * * * * * *
     * 0 0 0 0 *
     * 0 0 0 0 *
     * 0 0 0 0 *
     * * * * * *
     */

    Cells.Init(ECellGridType::Empty, Dim.Width * Dim.Height);
    InitWalls();
}

FDimension Grid::GetDimension() const
{
    return Dim;
}

void Grid::Update(const TPositionPtr* Links, ECellGridType CellType)
{
    FreeCellsByType(CellType);

    auto* Link = Links;
    while (Link)
    {
        const uint32 Index = PosToCellsIndex(Link->GetValue());

        Cells[Index] = CellType;
        IndexesByTypeMap[CellType].Add(Index);
        Link = Link->GetNextNode();
    }
}

bool Grid::HitTest(const FPosition& Position, ECellGridType CellType) const
{
    return Cells[PosToCellsIndex(Position)] == CellType;
}

void Grid::InitWalls()
{
    for (uint32 y = 0; y < Dim.Height; y++)
    {
        for (uint32 x = 0; x < Dim.Width; x++)
        {
            if (x == 0 || x == Dim.Width - 1 || y == 0 || y == Dim.Height - 1)
            {
                const uint32 Index = PosToCellsIndex(x, y);

                Cells[Index] = ECellGridType::Wall;
                IndexesByTypeMap[ECellGridType::Wall].Add(Index);
            }
        }
    }
}

void Grid::FreeCellsByType(ECellGridType CellType)
{
    if (!IndexesByTypeMap.Contains(CellType)) return;

    for (int32 i = 0; i < IndexesByTypeMap[CellType].Num(); ++i)
    {
        const uint32 Index = IndexesByTypeMap[CellType][i];
        Cells[Index] = ECellGridType::Empty;
    }
    IndexesByTypeMap[CellType].Empty();
}

uint32 Grid::PosToCellsIndex(const uint32 x, const uint32 y) const
{
    return x + y * Dim.Width;
}
uint32 Grid::PosToCellsIndex(const FPosition& Position) const
{
    return PosToCellsIndex(Position.x, Position.y);
}

FPosition Grid::GetCenter(const uint32 Width, const uint32 Height)
{
    return FPosition(Width / 2 + 1, Height / 2 + 1);
}
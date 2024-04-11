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

Grid::Grid(const FDimension& InDim, const IPositionRandomizerPtr& Randomizer)
    : GridPositionRandomizer(Randomizer), Dim(FDimension{InDim.Width + NumWallsInLine, InDim.Height + NumWallsInLine})
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

void Grid::Update(const TPositionPtr* Links, const ECellGridType& CellType)
{
    FreeCellsByType(CellType);

    auto* Link = Links;
    while (Link)
    {
        UpdateInternal(Link->GetValue(), CellType);
        Link = Link->GetNextNode();
    }
}
void Grid::Update(const FPosition& Position, const ECellGridType& CellType)
{
    FreeCellsByType(CellType);
    UpdateInternal(Position, CellType);
}

void Grid::UpdateInternal(const FPosition& Position, const ECellGridType& CellType)
{
    const uint32 Index = PosToIndex(Position);
    Cells[Index] = CellType;
    IndexesByTypeMap[CellType].Add(Index);
}

bool Grid::HitTest(const FPosition& Position, const ECellGridType& CellType) const
{
    return Cells[PosToIndex(Position)] == CellType;
}

UE_NODISCARD bool Grid::GetRandomEmptyPosition(FPosition& Position) const
{
    if (!GridPositionRandomizer) return false;
    return GridPositionRandomizer->GeneratePosition(Dim, Cells, Position);
}

void Grid::InitWalls()
{
    for (uint32 y = 0; y < Dim.Height; y++)
    {
        for (uint32 x = 0; x < Dim.Width; x++)
        {
            if (x == 0 || x == Dim.Width - 1 || y == 0 || y == Dim.Height - 1)
            {
                const uint32 Index = PosToIndex(x, y);

                Cells[Index] = ECellGridType::Wall;
                IndexesByTypeMap[ECellGridType::Wall].Add(Index);
            }
        }
    }
}

void Grid::FreeCellsByType(const ECellGridType& CellType)
{
    if (!IndexesByTypeMap.Contains(CellType)) return;

    for (int32 i = 0; i < IndexesByTypeMap[CellType].Num(); ++i)
    {
        const uint32 Index = IndexesByTypeMap[CellType][i];
        Cells[Index] = ECellGridType::Empty;
    }
    IndexesByTypeMap[CellType].Empty();
}

uint32 Grid::PosToIndex(const uint32 x, const uint32 y) const
{
    return x + y * Dim.Width;
}
uint32 Grid::PosToIndex(const FPosition& Position) const
{
    return PosToIndex(Position.x, Position.y);
}

FPosition Grid::GetCenter(const uint32 Width, const uint32 Height)
{
    return FPosition(Width / 2 + 1, Height / 2 + 1);
}
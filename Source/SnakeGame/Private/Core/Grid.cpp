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

void Grid::InitWalls()
{
    for (uint32 y = 0; y < Dim.Height; y++)
    {
        for (uint32 x = 0; x < Dim.Width; x++)
        {
            if (x == 0 || x == Dim.Width - 1 || y == 0 || y == Dim.Height - 1)
            {
                Cells[PosToCellsIndex(x, y)] = ECellGridType::Wall;
            }
        }
    }
}

uint32 Grid::PosToCellsIndex(const uint32 x, const uint32 y) const
{
    return x + y * Dim.Width;
}

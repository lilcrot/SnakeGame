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

    PrintDebug();
}

FDimension Grid::GetDimension() const
{
    return Dim;
}

void Grid::InitWalls()
{
    for (int32 y = 0; y < Dim.Height; y++)
    {
        for (int32 x = 0; x < Dim.Width; x++)
        {
            if (x == 0 || x == Dim.Width - 1 || y == 0 || y == Dim.Height - 1)
            {
                Cells[PosToCellsIndex(x, y)] = ECellGridType::Wall;
            }
        }
    }
}

void Grid::PrintDebug()
{
#if !UE_BUILD_SHIPPING
    for (int32 y = 0; y < Dim.Height; y++)
    {
        FString Line;
        for (int32 x = 0; x < Dim.Width; x++)
        {
            TCHAR Symbol{};
            switch (Cells[PosToCellsIndex(x, y)])
            {
                case ECellGridType::Empty:
                {
                    Symbol = '0';
                    break;
                }
                case ECellGridType::Wall:
                {
                    Symbol = '*';
                    break;
                }
            }
            Line.AppendChar(Symbol).AppendChar(' ');
        }
        UE_LOG(LogGrid, Display, TEXT("%s"), *Line);
    }
#endif
}

int32 Grid::PosToCellsIndex(const int32 x, const int32 y) const
{
    return x + y * Dim.Width;
}

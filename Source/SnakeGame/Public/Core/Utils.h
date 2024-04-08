// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class IPositionRandomizer
{
public:
    virtual bool GeneratePosition(const FDimension& Dim, const TArray<ECellGridType>& Cells, FPosition& Position) const = 0;
};

class PositionRandomizer : public IPositionRandomizer
{
public:
    virtual bool GeneratePosition(const FDimension& Dim, const TArray<ECellGridType>& Cells, FPosition& Position) const override
    {
        const uint32 GridSize = Dim.Width * Dim.Height;
        const uint32 Index = FMath::RandRange(0, GridSize - 1);

        for (uint32 i = Index; i < GridSize; ++i)
        {
            if (Cells[i] == ECellGridType::Empty)
            {
                Position = IndexToPos(i, Dim);
                return true;
            }
        }

        for (uint32 i = 0; i < Index; ++i)
        {
            if (Cells[i] == ECellGridType::Empty)
            {
                Position = IndexToPos(i, Dim);
                return true;
            }
        }

        return false;
    }

private:
    FPosition IndexToPos(const uint32 Index, const FDimension& Dim) const 
    { 
        return FPosition(Index % Dim.Width, Index / Dim.Width); 
    }
};

using IPositionRandomizerPtr = TSharedPtr<IPositionRandomizer>;

}  // namespace SnakeGame
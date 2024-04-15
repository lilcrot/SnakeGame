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
    virtual ~IPositionRandomizer() = default;
};

class PositionRandomizer : public IPositionRandomizer
{
public:
    virtual bool GeneratePosition(const FDimension& Dim, const TArray<ECellGridType>& Cells, FPosition& Position) const override
    {
        constexpr int32 NumWallsInLine = 2;

        const uint32 StartX = FMath::RandRange(1, Dim.Width - NumWallsInLine);
        const uint32 StartY = FMath::RandRange(1, Dim.Height - NumWallsInLine);
        FPosition RandomPosition = {StartX, StartY};

        do
        {
            const uint32 CurrentIndex = PosToIndex(RandomPosition, Dim);
            if (Cells[CurrentIndex] == ECellGridType::Empty)
            {
                Position = RandomPosition;
                return true;
            }

            if (++RandomPosition.x > Dim.Width - NumWallsInLine)
            {
                RandomPosition.x = 1;
                if (++RandomPosition.y > Dim.Height - NumWallsInLine)
                {
                    RandomPosition.y = 1;
                }
            }
        } while (RandomPosition.x != StartX || RandomPosition.y != StartY);

        return false;
    }

private:
    FPosition IndexToPos(const uint32 Index, const FDimension& Dim) const { return FPosition(Index % Dim.Width, Index / Dim.Width); }
    uint32 PosToIndex(const FPosition& Position, const FDimension& Dim) const { return Position.x + Position.y * Dim.Width; }
};

using IPositionRandomizerPtr = TSharedPtr<IPositionRandomizer>;

}  // namespace SnakeGame
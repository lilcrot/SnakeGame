// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Food
{
public:
    Food() = default;

    void SetPosition(const FPosition& InPosition);

    FPosition GetPosition() const;

private:
    FPosition Position;
};
}  // namespace SnakeGame

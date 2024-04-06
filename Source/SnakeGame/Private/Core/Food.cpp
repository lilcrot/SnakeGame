// Educational project Snake Game
#include "Core/Food.h"

using namespace SnakeGame;

void Food::SetPosition(const FPosition& InPosition)
{
    Position = InPosition;
}

FPosition Food::GetPosition() const
{
    return Position;
}

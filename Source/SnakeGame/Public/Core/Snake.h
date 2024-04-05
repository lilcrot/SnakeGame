// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Snake
{
public:
    Snake(const FSettings::FSnake& Settings);

    const TSnakeList& GetLinks() const;
    FPosition GetHead() const; 
    const TPositionPtr* GetBody() const;

    void Move(const FInput& Input);

private:
    TSnakeList Links;
    FInput LastInput{1, 0};
};
}  // namespace SnakeGame

// Educational project Snake Game

#include "Core/Snake.h"

using namespace SnakeGame;

Snake::Snake(const FSettings::FSnake& Settings)
{
    checkf(Settings.DefaultSize >= 4, TEXT("Snake length is too small: %i"), Settings.DefaultSize);

    // add snake links horizontally to the left [ ----* ]
    const FPosition StartPos = Settings.StartPosition;
    for (uint32 i = 0; i < Settings.DefaultSize; ++i)
    {
        Links.AddTail(FPosition{StartPos.x - i, StartPos.y});
    }
}

void Snake::Move(const FInput& Input)
{
    // can't move to opposite direction
    if (!LastInput.IsOpposite(Input))
    {
        LastInput = Input;
    }

    Links.GetTail()->GetValue() = Links.GetHead()->GetValue();
    Links.MoveTailAfterHead();
    Links.GetHead()->GetValue() += FPosition(LastInput.x, LastInput.y);
}

void Snake::IncreaseLength()
{
    Links.AddTail(Links.GetTail()->GetValue());
}

const TSnakeList& Snake::GetLinks() const
{
    return Links;
}

FPosition Snake::GetHeadPosition() const
{
    return Links.GetHead()->GetValue();
}

FPosition Snake::GetTailPosition() const
{
    return Links.GetTail()->GetValue();
}

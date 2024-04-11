// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "SnakeGame/Public/Containers/List.h"

namespace SnakeGame
{
struct FDimension
{
    uint32 Width = 0;
    uint32 Height = 0;

    bool operator==(const FDimension& rhs) const { return Width == rhs.Width && Height == rhs.Height; }
};

struct FPosition
{
    FPosition(uint32 inX, uint32 inY) : x(inX), y(inY) {}
    FPosition(const FPosition& position = FPosition::Zero) : x(position.x), y(position.y) {}

    uint32 x; /* possible values: (-1, 0, 1) */
    uint32 y; /* possible values: (-1, 0, 1) */

    FPosition& operator+=(const FPosition& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    bool operator==(const FPosition& rhs) const { return x == rhs.x && y == rhs.y; }

    static const FPosition Zero;
};

struct FInput
{
    int8 x;
    int8 y;

    bool IsOpposite(const FInput& rhs) const  //
    {
        return (x == -rhs.x && x != 0) || (y == -rhs.y && y != 0);
    }

    static const FInput Default;
};

enum class ECellGridType
{
    Empty = 0,
    Wall,
    Snake,
    Food
};
struct FSettings
{
    FDimension GridDimension{10, 10};

    struct FSnake
    {
        uint32 DefaultSize = 4;
        FPosition StartPosition{6, 6};  // Center of the GridDimension{10, 10} including walls
    } SnakeConfiguration;

    float GameSpeed = 1.0f;
};

using TSnakeList = TDoubleLinkedList<FPosition>;
using TPositionPtr = TSnakeList::TDoubleLinkedListNode;

enum class EGameplayEvent
{
    GameOver = 0,
    GameCompleted,
    FoodTaken
};

using GameplayEventCallback = TFunction<void(EGameplayEvent)>;

}  // namespace SnakeGame
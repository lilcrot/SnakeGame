// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"

namespace SnakeGame
{
struct FDimension
{
    uint32 Width = 0;
    uint32 Height = 0;
};

struct FPosition
{
    FPosition(uint32 inX, uint32 inY) : x(inX), y(inY) {}
    uint32 x;
    uint32 y;

    FPosition& operator+=(const FPosition& obj)
    {
        x += obj.x;
        y += obj.y;
        return *this;
    }
};

struct FInput
{
    int8 x;
    int8 y;

    bool IsOpposite(const FInput& rhs) const  //
    {
        return (x == -rhs.x && x != 0) || (y == -rhs.y && y != 0);
    }
};

enum class ECellGridType
{
    Empty = 0,
    Wall = 1,
    Snake = 2
};
struct FSettings
{
    FDimension GridDimension{40, 10};

    struct FSnake
    {
        uint32 DefaultSize = 4;
        FPosition StartPosition{0, 0};
    } SnakeConfiguration;

    float GameSpeed = 1.0f;
};

using TPositionPtr = TDoubleLinkedList<FPosition>::TDoubleLinkedListNode;

class TSnakeList : public TDoubleLinkedList<FPosition>
{
public:
    void MoveTail(TPositionPtr* Tail, TPositionPtr* Head, const FPosition& Pos)
    {
        // @todo: make real movement of tail node without remove/insert
        RemoveNode(Tail);
        InsertNode(Pos, Head->GetNextNode());
    }
};

}  // namespace SnakeGame
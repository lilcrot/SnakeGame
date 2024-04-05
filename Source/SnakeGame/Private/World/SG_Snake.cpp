// Educational project Snake Game

#include "World/SG_Snake.h"
#include "World/SG_SnakeLink.h"

namespace
{
FVector LinkPositionToVector(const SnakeGame::FPosition& Position, uint32 CellSize, const SnakeGame::FDimension& Dim)
{
    return FVector((Dim.Height - 1 - Position.y) * CellSize, Position.x * CellSize, 0.0) + FVector(CellSize * 0.5);
}
}  // namespace

ASG_Snake::ASG_Snake()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::FDimension& Dimension)
{
    MySnake = InSnake;
    CellSize = InCellSize;
    Dim = Dimension;
}

void ASG_Snake::UpdateColors(const FSnakeColors& Colors)
{
    for (int32 i = 0; i < SnakeLinks.Num(); i++)
    {
        SnakeLinks[i]->UpdateColors(i == 0 ? Colors.SnakeHeadColor : Colors.SnakeBodyColor);
    }
}

void ASG_Snake::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!MySnake.IsValid() || !World) return;

    const auto& Links = MySnake.Pin()->GetLinks();

    uint32 i = 0;
    for (const auto& Link : Links)
    {
        const bool bIsHead = i == 0;

        const FTransform Transform = FTransform(LinkPositionToVector(Link, CellSize, Dim));

        auto* LinkActor = World->SpawnActorDeferred<ASG_SnakeLink>(bIsHead ? SnakeHeadClass : SnakeBodyClass, Transform);
        LinkActor->UpdateScale(CellSize);
        LinkActor->FinishSpawning(Transform);
        SnakeLinks.Add(LinkActor);

        ++i;
    }
}

void ASG_Snake::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!MySnake.IsValid()) return;

    const auto& Links = MySnake.Pin()->GetLinks();
    auto* LinkPtr = Links.GetHead();

    for (auto* LinkActor : SnakeLinks)
    {
        LinkActor->SetActorLocation(LinkPositionToVector(LinkPtr->GetValue(), CellSize, Dim));
        LinkPtr = LinkPtr->GetNextNode();
    }
}

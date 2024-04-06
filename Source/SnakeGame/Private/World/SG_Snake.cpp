// Educational project Snake Game

#include "World/SG_Snake.h"
#include "World/SG_SnakeLink.h"
#include "World/SG_WorldUtils.h"
#include "Core/Snake.h" 
#include "World/SG_WorldTypes.h"
#include "Core/Types.h"

ASG_Snake::ASG_Snake()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::FDimension& Dimension)
{

    MySnake = InSnake;
    CellSize = InCellSize;
    Dim = Dimension;

    for (auto* LinkActor : SnakeLinks)
    {
        LinkActor->Destroy();
    }
    SnakeLinks.Empty();

    if (!MySnake.IsValid()) return;

    const auto& Links = MySnake.Pin()->GetLinks();

    uint32 i = 0;
    for (const auto& Link : Links)
    {
        const bool bIsHead = i == 0;
        SpawnSnakeLink(bIsHead ? SnakeHeadClass : SnakeBodyClass, Link);

        ++i;
    }
}

void ASG_Snake::UpdateColors(const FSnakeColors& Colors)
{
    SnakeBodyColor = Colors.SnakeBodyColor;

    for (int32 i = 0; i < SnakeLinks.Num(); i++)
    {
        SnakeLinks[i]->UpdateColor(i == 0 ? Colors.SnakeHeadColor : Colors.SnakeBodyColor);
    }
}

void ASG_Snake::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UWorld* World = GetWorld();
    if (!MySnake.IsValid() || !World) return;

    const auto& Links = MySnake.Pin()->GetLinks();
    auto* LinkPtr = Links.GetHead();

    for (auto* LinkActor : SnakeLinks)
    {
        LinkActor->SetActorLocation(SnakeGame::WorldUtils::PositionToVector(LinkPtr->GetValue(), CellSize, Dim));
        LinkPtr = LinkPtr->GetNextNode();
    }

    // add links if snake ate food
    while (LinkPtr)
    {
        SpawnSnakeLink(SnakeBodyClass, LinkPtr->GetValue());

        LinkPtr = LinkPtr->GetNextNode();
    }
}

ASG_SnakeLink* ASG_Snake::SpawnSnakeLink(UClass* Class, const SnakeGame::FPosition& Position)
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    const FTransform Transform = FTransform(SnakeGame::WorldUtils::PositionToVector(Position, CellSize, Dim));
    auto* LinkActor = World->SpawnActorDeferred<ASG_SnakeLink>(Class, Transform);
    LinkActor->UpdateScale(CellSize);
    LinkActor->FinishSpawning(Transform);
    LinkActor->UpdateColor(SnakeBodyColor);

    SnakeLinks.Add(LinkActor);

    return LinkActor;
}
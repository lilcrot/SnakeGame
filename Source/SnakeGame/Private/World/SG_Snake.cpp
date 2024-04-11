// Educational project Snake Game

#include "World/SG_Snake.h"
#include "World/SG_SnakeLink.h"
#include "World/SG_WorldUtils.h"
#include "Core/Snake.h"
#include "World/SG_WorldTypes.h"
#include "Core/Types.h"
#include "World/SG_ObjectPool.h"

ASG_Snake::ASG_Snake()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::FDimension& Dimension)
{
    InitObjectPool();

    MySnake = InSnake;
    CellSize = InCellSize;
    Dim = Dimension;

    for (const auto& LinkActor : SnakeLinks)
    {
        SnakeObjectPool->Add<ASG_SnakeLink>(LinkActor);
    }
    SnakeLinks.Empty();

    if (!MySnake.IsValid()) return;

    const auto& Links = MySnake.Pin()->GetLinks();

    for (const auto& Link : Links)
    {
        SpawnSnakeLink(SnakeLinkClass, Link);
    }
}

void ASG_Snake::InitObjectPool()
{
    constexpr int32 ReservedSnakeLinksNum = 10;

    if (SnakeObjectPool) return;

    SnakeObjectPool = NewObject<USG_ObjectPool>();
    checkf(SnakeObjectPool, TEXT("SnakeObjectPool is failed to create so InitObjectPool is failed"));
    SnakeObjectPool->Reserve<ASG_SnakeLink>(GetWorld(), ReservedSnakeLinksNum, SnakeLinkClass);
}

void ASG_Snake::UpdateColors(const FSnakeColors& Colors)
{
    SnakeBodyColor = Colors.SnakeBodyColor;

    for (int32 i = 0; i < SnakeLinks.Num(); i++)
    {
        const bool bIsHead = i == 0;
        SnakeLinks[i]->UpdateColor(bIsHead ? Colors.SnakeHeadColor : Colors.SnakeBodyColor);
    }
}

void ASG_Snake::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UWorld* World = GetWorld();
    if (!MySnake.IsValid() || !World) return;

    const auto& Links = MySnake.Pin()->GetLinks();
    auto* LinkPtr = Links.GetHead();

    for (const auto& LinkActor : SnakeLinks)
    {
        LinkActor->SetActorLocation(SnakeGame::WorldUtils::PositionToVector(LinkPtr->GetValue(), CellSize, Dim));
        LinkPtr = LinkPtr->GetNextNode();
    }

    // add links if snake ate food
    while (LinkPtr)
    {
        SpawnSnakeLink(SnakeLinkClass, LinkPtr->GetValue());

        LinkPtr = LinkPtr->GetNextNode();
    }
}

ASG_SnakeLink* ASG_Snake::SpawnSnakeLink(UClass* Class, const SnakeGame::FPosition& Position)
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    const FTransform Transform = FTransform(SnakeGame::WorldUtils::PositionToVector(Position, CellSize, Dim));
    auto* LinkActor = SnakeObjectPool->Pop<ASG_SnakeLink>(GetWorld(), Transform, SnakeLinkClass);

    LinkActor->UpdateScale(CellSize);
    LinkActor->UpdateColor(SnakeBodyColor);

    SnakeLinks.Add(LinkActor);

    return LinkActor;
}

void ASG_Snake::PlayExplodeEffect()
{
    for (const auto& LinkActor : SnakeLinks)
    {
        LinkActor->PlayExplodeEffect();
    }
}
// Educational project Snake Game
#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "Components/LineBatchComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldGrid, All, All);

ASG_Grid::ASG_Grid()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Grid::BeginPlay()
{
    Super::BeginPlay();
}

void ASG_Grid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    DrawGrid();
}

void ASG_Grid::SetModel(const TSharedPtr<SnakeGame::Grid>& Grid, const int32 InCellSize)
{
    if (!Grid.IsValid())
    {
        UE_LOG(LogWorldGrid, Fatal, TEXT("Grid is null, game aborted!"));
    }

    GridDimension = Grid.Get()->GetDimension();
    CellSize = InCellSize;
    WorldWidth = GridDimension.Width * CellSize;
    WorldHeight = GridDimension.Height * CellSize;
}

void ASG_Grid::DrawGrid()
{
    UWorld* World = GetWorld();
    if (!World || !World->LineBatcher) return;

    for (int32 i = 0; i < GridDimension.Height + 1; i++)
    {
        const FVector StartLocation = GetActorLocation() + GetActorForwardVector() * CellSize * i;
        const FVector EndLocation = StartLocation + GetActorRightVector() * WorldWidth;

        World->LineBatcher->DrawLine(StartLocation, EndLocation, FLinearColor::Red, 0, 2.0f);
    }

    for (int32 i = 0; i < GridDimension.Width + 1; i++)
    {
        const FVector StartLocation = GetActorLocation() + GetActorRightVector() * CellSize * i;
        const FVector EndLocation = StartLocation + GetActorForwardVector() * WorldHeight;

        World->LineBatcher->DrawLine(StartLocation, EndLocation, FLinearColor::Red, 0, 2.0f);
    }
}

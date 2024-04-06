// Educational project Snake Game
#include "World/SG_Food.h"
#include "Components/StaticMeshComponent.h"
#include "World/SG_WorldUtils.h"
#include "Core/Food.h"

ASG_Food::ASG_Food()
{
    PrimaryActorTick.bCanEverTick = true;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    checkf(Origin, TEXT("Origin is failed to create"));
    SetRootComponent(Origin);

    FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>("FoodMesh");
    checkf(FoodMesh, TEXT("FoodMesh is failed to create"));
    FoodMesh->SetupAttachment(Origin);
}

void ASG_Food::SetModel(const TSharedPtr<SnakeGame::Food>& InFood, const uint32 InCellSize, const SnakeGame::FDimension& InDim)
{
    Food = InFood;
    CellSize = InCellSize;
    Dim = InDim;

    SnakeGame::WorldUtils::ScaleMesh(FoodMesh, FVector(CellSize));
}

void ASG_Food::UpdateColor(const FLinearColor& Color)
{
    if (auto* FoodMaterial = FoodMesh->CreateAndSetMaterialInstanceDynamic(0))
    {
        FoodMaterial->SetVectorParameterValue("Color", Color);
    }
}

void ASG_Food::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Food.IsValid())
    {
        SetActorLocation(SnakeGame::WorldUtils::PositionToVector(Food.Pin()->GetPosition(), CellSize, Dim));
    }
}

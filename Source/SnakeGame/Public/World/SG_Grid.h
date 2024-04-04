// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Types.h"
#include "World/SG_WorldTypes.h"

#include "SG_Grid.generated.h"

namespace SnakeGame
{
class Grid;
}

class UStaticMeshComponent;

UCLASS()
class SNAKEGAME_API ASG_Grid : public AActor
{
    GENERATED_BODY()

public:
    ASG_Grid();

    void SetModel(const TSharedPtr<SnakeGame::Grid>& Grid, const uint32 InCellSize);

    void UpdateColors(const FSnakeColors& Colors);

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* GridMesh;

private:
    UPROPERTY()
    UMaterialInstanceDynamic* GridMaterial;

    SnakeGame::FDimension GridDimension;
    uint32 CellSize;
    uint32 WorldWidth;
    uint32 WorldHeight;
};
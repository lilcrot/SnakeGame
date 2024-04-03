// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Types.h"

#include "SG_Grid.generated.h"

namespace SnakeGame
{
class Grid;
}

UCLASS()
class SNAKEGAME_API ASG_Grid : public AActor
{
    GENERATED_BODY()

public:
    ASG_Grid();

    virtual void Tick(float DeltaTime) override;

    void SetModel(const TSharedPtr<SnakeGame::Grid>& Grid, const int32 InCellSize);

protected:
    virtual void BeginPlay() override;

private:
    SnakeGame::FDimension GridDimension;
    int32 CellSize;
    int32 WorldWidth;
    int32 WorldHeight;

    void DrawGrid();
};
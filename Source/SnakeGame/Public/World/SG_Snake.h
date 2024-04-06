// Educational project Snake Game
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Types.h"
#include "World/SG_WorldTypes.h"

#include "SG_Snake.generated.h"

namespace SnakeGame
{
class Snake;
}

class ASG_SnakeLink;

UCLASS()
class SNAKEGAME_API ASG_Snake : public AActor
{
    GENERATED_BODY()

public:
    ASG_Snake();
    virtual void Tick(float DeltaTime) override;

    void SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::FDimension& Dimension);
    void UpdateColors(const FSnakeColors& Colors);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ASG_SnakeLink> SnakeHeadClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ASG_SnakeLink> SnakeBodyClass;

private:
    ASG_SnakeLink* SpawnSnakeLink(UClass* Class, const SnakeGame::FPosition& Position);

private:
    TWeakPtr<SnakeGame::Snake> MySnake;
    uint32 CellSize;
    SnakeGame::FDimension Dim;
    FLinearColor SnakeBodyColor;

    UPROPERTY()
    TArray<ASG_SnakeLink*> SnakeLinks;
};

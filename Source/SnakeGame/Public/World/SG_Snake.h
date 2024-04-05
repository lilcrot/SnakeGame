// Educational project Snake Game
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Snake.h"
#include "World/SG_WorldTypes.h"

#include "SG_Snake.generated.h"


class ASG_SnakeLink;

UCLASS()
class SNAKEGAME_API ASG_Snake : public AActor
{
    GENERATED_BODY()

public:
    ASG_Snake();

    void SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::FDimension& Dimension);
    void UpdateColors(const FSnakeColors& Colors);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ASG_SnakeLink> SnakeHeadClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ASG_SnakeLink> SnakeBodyClass;

public:
    virtual void Tick(float DeltaTime) override;

private:
    TWeakPtr<SnakeGame::Snake> MySnake;
    uint32 CellSize;
    SnakeGame::FDimension Dim;

    UPROPERTY()
    TArray<ASG_SnakeLink*> SnakeLinks;
};

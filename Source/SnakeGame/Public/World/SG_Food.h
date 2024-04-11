// Educational project Snake Game
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Types.h"
#include "World/SG_WorldTypes.h"

#include "SG_Food.generated.h"

namespace SnakeGame
{
class Food;
}

class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class SNAKEGAME_API ASG_Food : public AActor
{
    GENERATED_BODY()

public:
    ASG_Food();
    virtual void Tick(float DeltaTime) override;

    void SetModel(const TSharedPtr<SnakeGame::Food>& InFood, const uint32 InCellSize, const SnakeGame::FDimension& InDim);

    void UpdateColor(const FLinearColor& Color);

    void PlayExplodeEffect();

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> Origin;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> FoodMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual")
    TObjectPtr<UNiagaraSystem> ExplosionEffect;

private:
    TWeakPtr<SnakeGame::Food> Food;
    uint32 CellSize;
    SnakeGame::FDimension Dim;

    FLinearColor FoodColor;

    FVector GetFoodWorldLocation() const;
};

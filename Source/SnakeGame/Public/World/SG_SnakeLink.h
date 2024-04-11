// Educational project Snake Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SG_SnakeLink.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS(Abstract)
class SNAKEGAME_API ASG_SnakeLink : public AActor
{
    GENERATED_BODY()

public:
    ASG_SnakeLink();

    void UpdateColor(const FLinearColor& Color);
    void UpdateScale(const uint32 CellSize);
    void PlayExplodeEffect();

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> Origin;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> LinkMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual")
    TObjectPtr<UNiagaraSystem> ExplosionEffect;

private:
    FLinearColor LinkColor;
};

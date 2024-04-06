// Educational project Snake Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SG_SnakeLink.generated.h"

class UStaticMeshComponent;

UCLASS(Abstract)
class SNAKEGAME_API ASG_SnakeLink : public AActor
{
    GENERATED_BODY()

public:
    ASG_SnakeLink();

    void UpdateColor(const FLinearColor& Color);
    void UpdateScale(const uint32 CellSize);

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* LinkMesh;
};

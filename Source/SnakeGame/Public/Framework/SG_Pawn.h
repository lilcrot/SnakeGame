// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/Types.h"

#include "SG_Pawn.generated.h"

class UCameraComponent;

UCLASS()
class SNAKEGAME_API ASG_Pawn : public APawn
{
    GENERATED_BODY()

public:
    ASG_Pawn();

    void UpdateLocation(const SnakeGame::FDimension& Dim, const int32 CellSize, const FTransform& GridOrigin);

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;

private:
    SnakeGame::FDimension Dim;
    int32 CellSize;
    FTransform GridOrigin;
    FDelegateHandle ResizeHandle;

    void OnViewportResized(FViewport* Viewport, uint32 Val);
};

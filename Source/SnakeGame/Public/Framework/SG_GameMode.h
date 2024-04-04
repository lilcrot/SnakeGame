// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Game.h"

#include "SG_GameMode.generated.h"

class ASG_Grid;
class AExponentialHeightFog;
class UDataTable;

UCLASS()
class SNAKEGAME_API ASG_GameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    virtual void StartPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"))
    FUintPoint GridDimension{10, 10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"))
    uint32 CellSize{10};

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ASG_Grid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    UDataTable* ColorsTable;

private:
    TUniquePtr<SnakeGame::Game> CoreGame;

    UPROPERTY()
    ASG_Grid* GridVisual;

    UFUNCTION(Exec, Category = "Console command")
    void NextColor();

    UPROPERTY()
    AExponentialHeightFog* Fog;

    uint32 ColorTableIndex{0};

    void FindFog();
    void UpdateColors();
};

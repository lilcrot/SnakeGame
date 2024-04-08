// Educational project Snake Game
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Game.h"
#include "InputActionValue.h"

#include "SG_GameMode.generated.h"

class ASG_Grid;
class ASG_Snake;
class ASG_Food;

class AExponentialHeightFog;
class UDataTable;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SNAKEGAME_API ASG_GameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ASG_GameMode();

    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"), Category = "Settings")
    FUintPoint GridDimension{10, 10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"), Category = "Settings")
    uint32 CellSize = 10;

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "4", ClampMax = "10"), Category = "Settings")
    uint32 SnakeDefaultSize = 5;

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.01", ClampMax = "10"), Category = "Settings")
    float GameSpeed = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    TSubclassOf<ASG_Grid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    TSubclassOf<ASG_Snake> SnakeVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    TSubclassOf<ASG_Food> FoodVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    TObjectPtr<UDataTable> ColorsTable;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> MoveForwardInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> MoveRightInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> ResetGameInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputMappingContext> InputMapping;

private:
    UPROPERTY()
    ASG_Grid* GridVisual;

    UPROPERTY()
    ASG_Snake* SnakeVisual;

    UPROPERTY()
    ASG_Food* FoodVisual;

    UFUNCTION(Exec, Category = "Console command")
    void NextColor();

    UPROPERTY()
    AExponentialHeightFog* Fog;

private:
    TUniquePtr<SnakeGame::Game> CoreGame;

    uint32 ColorTableIndex = 0;

    SnakeGame::FInput SnakeInput = SnakeGame::FInput::Default;

    SnakeGame::FSettings MakeSettings() const;

    void FindFog();
    void UpdateColors();
    void SetupInput();
    void OnMoveForward(const FInputActionValue& Value);
    void OnMoveRight(const FInputActionValue& Value);
    void OnGameReset(const FInputActionValue& Value);

    void SubscribeOnGameEvents();
};

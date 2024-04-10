// Educational project Snake Game
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SG_HUD.generated.h"

class USG_GameplayWidget;
class USG_GameOverWidget;

UENUM()
enum class EUIGameState : uint8
{
    GameInProgress = 0,
    GameOver,
    GameCompleted
};

namespace SnakeGame
{
class Game;
}  // namespace SnakeGame

UCLASS(Abstract)
class SNAKEGAME_API ASG_HUD : public AHUD
{
    GENERATED_BODY()

public:
    ASG_HUD();

    void SetModel(const TSharedPtr<SnakeGame::Game>& Game);
    void SetInputKeyNames(const FString& ResetGameKeyName);

    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USG_GameplayWidget> GameplayWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USG_GameOverWidget> GameOverWidgetClass;

private:
    UPROPERTY()
    TObjectPtr<USG_GameplayWidget> GameplayWidget;

    UPROPERTY()
    TObjectPtr<USG_GameOverWidget> GameOverWidget;

    UPROPERTY()
    TMap<EUIGameState, TObjectPtr<UUserWidget>> GameWidgets;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget;

    TWeakPtr<SnakeGame::Game> Game;
    EUIGameState GameState;

    void SetUIGameState(const EUIGameState& GameState);
};

// Educational project Snake Game
#include "UI/SG_HUD.h"
#include "UI/SG_GameplayWidget.h"
#include "UI/SG_GameOverWidget.h"
#include "Core/Game.h"

ASG_HUD::ASG_HUD()
{
    PrimaryActorTick.TickInterval = 1.0f;
}

void ASG_HUD::BeginPlay()
{
    Super::BeginPlay();

    GameplayWidget = CreateWidget<USG_GameplayWidget>(GetWorld(), GameplayWidgetClass);
    checkf(GameplayWidget, TEXT("GameplayWidget is failed to create"));
    GameWidgets.Add(EUIGameState::GameInProgress, GameplayWidget);

    GameOverWidget = CreateWidget<USG_GameOverWidget>(GetWorld(), GameOverWidgetClass);
    checkf(GameOverWidget, TEXT("GameOverWidget is failed to create"));
    GameWidgets.Add(EUIGameState::GameOver, GameOverWidget);

    for (auto& [UIState, GameWidget] : GameWidgets)
    {
        if (GameWidget)
        {
            GameWidget->AddToViewport();
            GameWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void ASG_HUD::SetModel(const TSharedPtr<SnakeGame::Game>& InGame)
{
    if (!InGame) return;
    using namespace SnakeGame;

    Game = InGame;

    SetUIGameState(EUIGameState::GameInProgress);

    InGame->SubscribeOnGameplayEvent(
        [&](const EGameplayEvent& Event)
        {
            switch (Event)
            {
                case EGameplayEvent::FoodTaken:
                {
                    GameplayWidget->SetScore(InGame->GetScore());
                    break;
                }

                case EGameplayEvent::GameCompleted: [[fallthrough]];
                case EGameplayEvent::GameOver:
                {
                    GameOverWidget->SetScore(InGame->GetScore());
                    SetUIGameState(EUIGameState::GameOver);
                    break;
                }
            }
        });
}

void ASG_HUD::SetInputKeyNames(const FString& ResetGameKeyName)
{
    GameplayWidget->SetResetGameKeyName(ResetGameKeyName);
    GameOverWidget->SetResetGameKeyName(ResetGameKeyName);
}

void ASG_HUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (Game.IsValid() && GameState == EUIGameState::GameInProgress)
    {
        GameplayWidget->SetGameTime(Game.Pin()->GetGameTime());
    }
}

void ASG_HUD::SetUIGameState(const EUIGameState& InGameState)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (GameWidgets.Contains(InGameState))
    {
        CurrentWidget = GameWidgets[InGameState];
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }

    if (InGameState == EUIGameState::GameInProgress && Game.IsValid())
    {
        GameplayWidget->SetScore(Game.Pin()->GetScore());
        GameOverWidget->SetScore(Game.Pin()->GetScore());
    }

    GameState = InGameState;
}

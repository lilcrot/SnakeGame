// Educational project Snake Game
#include "Framework/SG_GameMode.h"
#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Types.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "World/SG_Food.h"
#include "Framework/SG_Pawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "World/SG_WorldTypes.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "UI/SG_HUD.h"
#include "World/SG_WorldUtils.h"
#include "Framework/SG_GameUserSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogSnakeGameMode, All, All);

ASG_GameMode::ASG_GameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    /* init core game */
    {
        CoreGame = MakeShared<SnakeGame::Game>(MakeSettings());
        checkf(CoreGame.IsValid(), TEXT("CoreGame isn't valid!"));
        SubscribeOnGameEvents();
    }

    UWorld* World = GetWorld();
    checkf(World, TEXT("World doesn't exist"));

    const auto* PlayerController = World->GetFirstPlayerController();
    checkf(PlayerController, TEXT("FirstPlayerController doesn't exist"));

    const FTransform GridOrigin = FTransform::Identity;

    /* init world grid */
    {
        GridVisual = World->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
        checkf(GridVisual, TEXT("GridVisual failed to spawn"));
        GridVisual->SetModel(CoreGame->GetGrid(), CellSize);

        GridVisual->FinishSpawning(GridOrigin);
    }

    /* init world snake */
    {
        SnakeVisual = World->SpawnActorDeferred<ASG_Snake>(SnakeVisualClass, GridOrigin);
        SnakeVisual->SetModel(CoreGame->GetSnake(), CellSize, CoreGame->GetGrid()->GetDimension());
        SnakeVisual->FinishSpawning(GridOrigin);
    }

    /* init world food */
    {
        FoodVisual = World->SpawnActorDeferred<ASG_Food>(FoodVisualClass, GridOrigin);
        FoodVisual->SetModel(CoreGame->GetFood(), CellSize, CoreGame->GetGrid()->GetDimension());
        FoodVisual->FinishSpawning(GridOrigin);
    }

    /* set pawn location fitting grid in viewport */
    {

        auto* Pawn = Cast<ASG_Pawn>(PlayerController->GetPawn());
        checkf(Pawn, TEXT("Pawn doesn't exist"));
        checkf(CoreGame->GetGrid().IsValid(), TEXT("GameGrid of the CoreGame doesn't exist"));

        Pawn->UpdateLocation(CoreGame->GetGrid()->GetDimension(), CellSize, GridOrigin);
    }

    /* update colors */
    {
        FindFog();

        checkf(ColorsTable, TEXT("ColorsTable isn't set"));
        const int32 RowsCount = ColorsTable->GetRowNames().Num();
        checkf(RowsCount >= 1, TEXT("ColorsTable is empty"));

        ColorTableIndex = FMath::RandRange(0, RowsCount - 1);
        UpdateColors();
    }

    SetupInput();

    /* init HUD */
    {
        HUD = Cast<ASG_HUD>(PlayerController->GetHUD());
        checkf(HUD, TEXT("SG_HUD isn't valid"));

        HUD->SetModel(CoreGame);
        const FString ResetGameKeyName = SnakeGame::WorldUtils::FindActionKeyName(InputMapping, ResetGameInputAction);
        HUD->SetInputKeyNames(ResetGameKeyName);

        SnakeGame::WorldUtils::SetUIInput(GetWorld(), false);
    }
}

void ASG_GameMode::NextColor()
{
    if (ColorsTable)
    {
        ColorTableIndex = (ColorTableIndex + 1) % ColorsTable->GetRowNames().Num();
        UpdateColors();

        SnakeGame::WorldUtils::SetUIInput(GetWorld(), false);
    }
}

void ASG_GameMode::FindFog()
{
    TArray<AActor*> Fogs;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExponentialHeightFog::StaticClass(), Fogs);
    if (Fogs.Num() > 0)
    {
        Fog = Cast<AExponentialHeightFog>(Fogs[0]);
    }
}

void ASG_GameMode::UpdateColors()
{
    const FName RowName = ColorsTable->GetRowNames()[ColorTableIndex];
    const auto* ColorSet = ColorsTable->FindRow<FSnakeColors>(RowName, {});
    if (ColorSet)
    {
        GridVisual->UpdateColors(*ColorSet);
        SnakeVisual->UpdateColors(*ColorSet);
        FoodVisual->UpdateColor(ColorSet->FoodColor);

        // update scene ambient color via fog
        if (Fog && Fog->GetComponent())
        {
            Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorSet->SkyAtmosphereColor;
            Fog->MarkComponentsRenderStateDirty();
        }
    }
}

void ASG_GameMode::SetupInput()
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (auto* PC = World->GetFirstPlayerController())
    {
        if (auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(InputMapping, 0);
        }

        auto* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PC->InputComponent);
        EnhancedInputComponent->BindAction(MoveForwardInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveForward);
        EnhancedInputComponent->BindAction(MoveRightInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveRight);
        EnhancedInputComponent->BindAction(ResetGameInputAction, ETriggerEvent::Started, this, &ThisClass::OnGameReset);
    }
}

void ASG_GameMode::OnMoveForward(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    if (InputValue == 0.0) return;

    SnakeInput = SnakeGame::FInput{0, static_cast<int8>(InputValue)};
}

void ASG_GameMode::OnMoveRight(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    if (InputValue == 0.0) return;

    SnakeInput = SnakeGame::FInput{static_cast<int8>(InputValue), 0};
}

void ASG_GameMode::OnGameReset(const FInputActionValue& Value)
{
    if (const bool bIsPressed = Value.Get<bool>())
    {
        CoreGame = MakeShared<SnakeGame::Game>(MakeSettings());
        checkf(CoreGame.IsValid(), TEXT("CoreGame isn't valid!"));
        SubscribeOnGameEvents();

        GridVisual->SetModel(CoreGame->GetGrid(), CellSize);
        SnakeVisual->SetModel(CoreGame->GetSnake(), CellSize, CoreGame->GetGrid()->GetDimension());
        FoodVisual->SetModel(CoreGame->GetFood(), CellSize, CoreGame->GetGrid()->GetDimension());
        HUD->SetModel(CoreGame);

        SnakeInput = SnakeGame::FInput::Default;
        NextColor();
    }
}

void ASG_GameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (CoreGame.IsValid())
    {
        CoreGame->Update(DeltaSeconds, SnakeInput);
    }
}

SnakeGame::FSettings ASG_GameMode::MakeSettings() const
{
    SnakeGame::FSettings GS;

#if WITH_EDITOR
    if (bOverrideUserSettings)
    {
        GS.GridDimension = SnakeGame::FDimension{GridDimension.X, GridDimension.Y};
        GS.GameSpeed = GameSpeed;
    }
    else
#endif
        if (const auto* UserSettings = USG_GameUserSettings::Get())
    {
        GS.GridDimension = UserSettings->GetGridSize();
        GS.GameSpeed = UserSettings->GetGameSpeed();
    }

    GS.SnakeConfiguration.DefaultSize = SnakeDefaultSize;
    GS.SnakeConfiguration.StartPosition = SnakeGame::Grid::GetCenter(GS.GridDimension.Width, GS.GridDimension.Height);

    return GS;
}

void ASG_GameMode::SubscribeOnGameEvents()
{
    using namespace SnakeGame;

    CoreGame->SubscribeOnGameplayEvent(
        [&](const EGameplayEvent& Event)
        {
            switch (Event)
            {
                case EGameplayEvent::GameOver:
                {
                    SnakeVisual->PlayExplodeEffect();
                    FoodVisual->SetActorHiddenInGame(true);
                    WorldUtils::SetUIInput(GetWorld(), true);

                    break;
                }

                case EGameplayEvent::GameCompleted:
                {
                    break;
                }

                case EGameplayEvent::FoodTaken:
                {
                    FoodVisual->PlayExplodeEffect();
                    break;
                }
            }
        });
}

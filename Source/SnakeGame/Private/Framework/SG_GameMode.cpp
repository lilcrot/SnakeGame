// Educational project Snake Game
#include "Framework/SG_GameMode.h"
#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Types.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "Framework/SG_Pawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "World/SG_WorldTypes.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ASG_GameMode::ASG_GameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    /* init core game */
    {
        CoreGame = MakeUnique<SnakeGame::Game>(MakeSettings());
        checkf(CoreGame.IsValid(), TEXT("CoreGame isn't valid!"));
    }

    UWorld* World = GetWorld();
    checkf(World, TEXT("World doesn't exist"));

    const FTransform GridTransform = FTransform::Identity;

    /* init world grid */
    {
        GridVisual = World->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridTransform);
        checkf(GridVisual, TEXT("GridVisual failed to spawn"));
        GridVisual->SetModel(CoreGame->GetGrid(), CellSize);

        GridVisual->FinishSpawning(GridTransform);
    }

    /* init world snake */
    {
        SnakeVisual = World->SpawnActorDeferred<ASG_Snake>(SnakeVisualClass, GridTransform);
        SnakeVisual->SetModel(CoreGame->GetSnake(), CellSize, CoreGame->GetGrid()->GetDimension());
        SnakeVisual->FinishSpawning(GridTransform);
    }

    /* set pawn location fitting grid in viewport */
    {
        const auto* PlayerController = World->GetFirstPlayerController();
        checkf(PlayerController, TEXT("FirstPlayerController doesn't exist"));

        auto* Pawn = Cast<ASG_Pawn>(PlayerController->GetPawn());
        checkf(Pawn, TEXT("Pawn doesn't exist"));
        checkf(CoreGame->GetGrid().IsValid(), TEXT("GameGrid of the CoreGame doesn't exist"));

        Pawn->UpdateLocation(CoreGame->GetGrid()->GetDimension(), CellSize, GridTransform);
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
}

void ASG_GameMode::NextColor()
{
    if (ColorsTable)
    {
        ColorTableIndex = (ColorTableIndex + 1) % ColorsTable->GetRowNames().Num();
        UpdateColors();
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
        CoreGame.Reset(new SnakeGame::Game(MakeSettings()));
        checkf(CoreGame.IsValid(), TEXT("CoreGame isn't valid!"));

        GridVisual->SetModel(CoreGame->GetGrid(), CellSize);
        SnakeVisual->SetModel(CoreGame->GetSnake(), CellSize, CoreGame->GetGrid()->GetDimension());
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
    GS.GridDimension = SnakeGame::FDimension{GridDimension.X, GridDimension.Y};
    GS.GameSpeed = GameSpeed;
    GS.SnakeConfiguration.DefaultSize = SnakeDefaultSize;
    GS.SnakeConfiguration.StartPosition = SnakeGame::Grid::GetCenter(GridDimension.X, GridDimension.Y);

    return GS;
}

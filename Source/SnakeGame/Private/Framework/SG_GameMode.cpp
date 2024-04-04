// Educational project Snake Game
#include "Framework/SG_GameMode.h"
#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Types.h"
#include "World/SG_Grid.h"
#include "Framework/SG_Pawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "World/SG_WorldTypes.h"

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    /* init core game */
    {
        const SnakeGame::FSettings GameSettings{GridDimension.X, GridDimension.Y};
        CoreGame = MakeUnique<SnakeGame::Game>(GameSettings);
        checkf(CoreGame.IsValid(), TEXT("CoreGame isn't valid!"));
    }

    UWorld* World = GetWorld();
    checkf(World, TEXT("World doesn't exist"));

    const FTransform GridTransform = FTransform::Identity;

    /* init world grid */
    {
        GridVisual = World->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridTransform);
        checkf(GridVisual, TEXT("GridVisual failed to spawn"));
        GridVisual->SetModel(CoreGame->GetGameGrid(), CellSize);

        GridVisual->FinishSpawning(GridTransform);
    }

    /* set pawn location fitting grid in viewport */
    {
        const auto* PlayerController = GetWorld()->GetFirstPlayerController();
        checkf(PlayerController, TEXT("FirstPlayerController doesn't exist"));

        auto* Pawn = Cast<ASG_Pawn>(PlayerController->GetPawn());
        checkf(Pawn, TEXT("Pawn doesn't exist"));
        checkf(CoreGame->GetGameGrid().IsValid(), TEXT("GameGrid of the CoreGame doesn't exist"));

        Pawn->UpdateLocation(CoreGame->GetGameGrid()->GetDimension(), CellSize, GridTransform);
    }

    /* update colors */
    {
        FindFog();

        checkf(ColorsTable, TEXT("ColorsTable isn't set"));
        const int32 RowsCount = ColorsTable->GetRowNames().Num();
        checkf(RowsCount >= 1, TEXT("ColorsTable is empty"))

        ColorTableIndex = FMath::RandRange(0, RowsCount - 1);
        UpdateColors();
    }
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
        // update grid
        GridVisual->UpdateColors(*ColorSet);

        // update scene ambient color via fog
        if (Fog && Fog->GetComponent())
        {
            Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorSet->SkyAtmosphereColor;
            Fog->MarkComponentsRenderStateDirty();
        }
    }
}
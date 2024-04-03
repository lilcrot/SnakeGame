// Educational project Snake Game
#include "Framework/SG_GameMode.h"
#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Types.h"
#include "World/SG_Grid.h"
#include "Framework/SG_Pawn.h"

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
}

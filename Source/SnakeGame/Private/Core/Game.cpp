// Educational project Snake Game
#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

using namespace SnakeGame;

Game::Game(const FSettings& SettingsIn)  //
    : Settings(SettingsIn)
{
    GameGrid = MakeShared<Grid>(Settings.GridDimension);
    GameSnake = MakeShared<Snake>(Settings.SnakeConfiguration);
    UpdateGrid();
}

TSharedPtr<Grid> Game::GetGrid() const
{
    return GameGrid;
}
TSharedPtr<Snake> Game::GetSnake() const
{
    return GameSnake;
}

void Game::Update(float DeltaSeconds, const FInput& Input)
{
    if (bIsGameOver || !CanUpdateTime(DeltaSeconds)) return;

    Move(Input);

    if (IsDied())
    {
        bIsGameOver = true;
        UE_LOG(LogGame, Display, TEXT("-------------- GAME OVER --------------"));
    }
}

void Game::Move(const FInput& Input)
{
    GameSnake->Move(Input);
    UpdateGrid();
}

void Game::UpdateGrid()
{
    GameGrid->Update(GameSnake->GetBody(), ECellGridType::Snake);
}

bool Game::CanUpdateTime(float DeltaSeconds)
{
    MoveSeconds += DeltaSeconds;
    if (MoveSeconds < Settings.GameSpeed) return false;

    MoveSeconds = 0.0f;
    return true;
}

bool Game::IsDied() const
{
    return GameGrid->HitTest(GameSnake->GetHead(), ECellGridType::Wall) ||  //
           GameGrid->HitTest(GameSnake->GetHead(), ECellGridType::Snake);
}

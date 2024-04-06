// Educational project Snake Game
#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"
#include "Core/Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

using namespace SnakeGame;

Game::Game(const FSettings& SettingsIn)  //
    : Settings(SettingsIn)
{
    GameGrid = MakeShared<Grid>(Settings.GridDimension);

    checkf(GameGrid->GetDimension().Width / 2 >= Settings.SnakeConfiguration.DefaultSize,                        //
        TEXT("Snake initial length [%i] doesn't fit grid width [%i]"), Settings.SnakeConfiguration.DefaultSize,  //
        GameGrid->GetDimension().Width);                                                                         //

    GameSnake = MakeShared<Snake>(Settings.SnakeConfiguration);
    GameFood = MakeShared<Food>();

    UpdateGrid();
    GenerateFood();
}

TSharedPtr<Grid> Game::GetGrid() const
{
    return GameGrid;
}
TSharedPtr<Snake> Game::GetSnake() const
{
    return GameSnake;
}
TSharedPtr<Food> Game::GetFood() const
{
    return GameFood;
}

void Game::Update(float DeltaSeconds, const FInput& Input)
{
    if (bIsGameOver || !CanUpdateTime(DeltaSeconds)) return;

    GameSnake->Move(Input);

    if (IsDied())
    {
        bIsGameOver = true;
        UE_LOG(LogGame, Display, TEXT("-------------- GAME OVER --------------"));
        UE_LOG(LogGame, Display, TEXT("-------------- SCORE: %i --------------"), Score);
        return;
    }

    if (IsFoodTaken())
    {
        ++Score;
        GameSnake->IncreaseLength();
        GenerateFood();
    }

    UpdateGrid();
}

void Game::UpdateGrid()
{
    GameGrid->Update(GameSnake->GetLinks().GetHead(), ECellGridType::Snake);
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
    return GameGrid->HitTest(GameSnake->GetHeadPosition(), ECellGridType::Wall) ||  //
           GameGrid->HitTest(GameSnake->GetHeadPosition(), ECellGridType::Snake);
}

void Game::GenerateFood()
{
    FPosition FoodPosition;
    if (GameGrid->GetRandomEmptyPosition(FoodPosition))
    {
        GameFood->SetPosition(FoodPosition);
        GameGrid->Update(GameFood->GetPosition(), ECellGridType::Food);
    }
    else
    {
        bIsGameOver = true;
        UE_LOG(LogGame, Display, TEXT("-------------- GAME COMPLETED --------------"));
        UE_LOG(LogGame, Display, TEXT("-------------- SCORE: %i --------------"), Score);
    }
}

bool Game::IsFoodTaken() const
{
    return GameGrid->HitTest(GameSnake->GetHeadPosition(), ECellGridType::Food);
}

// Educational project Snake Game
#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"
#include "Core/Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

using namespace SnakeGame;

Game::Game(const FSettings& SettingsIn, const IPositionRandomizerPtr& Randomizer)  //
    : Settings(SettingsIn)
{
    GameGrid = MakeShared<Grid>(Settings.GridDimension, Randomizer);

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


    // TODO: Improve HitTest to SnakeHead. Add to enum SnakeHead and update code...
    // then we can UpdateGrid immediately after update move for GameSnake
    const auto PrevTailPosition = GameSnake->GetTailPosition();
    GameSnake->Move(Input);

    if (IsDied(PrevTailPosition))
    {
        bIsGameOver = true;
        DispatchEvent(EGameplayEvent::GameOver);

        return;
    }

    if (IsFoodTaken())
    {
        ++Score;
        GameSnake->IncreaseLength();
        DispatchEvent(EGameplayEvent::FoodTaken);
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
    GameTime += DeltaSeconds;

    MoveSeconds += DeltaSeconds;
    if (MoveSeconds < Settings.GameSpeed) return false;

    MoveSeconds = 0.0f;
    return true;
}

bool Game::IsDied(const FPosition& PrevTailPosition) const
{
    if (GameGrid->HitTest(GameSnake->GetHeadPosition(), ECellGridType::Wall)) return true;
    if (GameSnake->GetHeadPosition() == PrevTailPosition) return false;  // corner case

    return GameGrid->HitTest(GameSnake->GetHeadPosition(), ECellGridType::Snake);
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
        DispatchEvent(EGameplayEvent::GameCompleted);
    }
}

bool Game::IsFoodTaken() const
{
    return GameGrid->HitTest(GameSnake->GetHeadPosition(), ECellGridType::Food);
}

void Game::SubscribeOnGameplayEvent(const GameplayEventCallback Callback)
{
    GameEventCallbacks.Add(Callback);
}

void Game::DispatchEvent(const EGameplayEvent& Event)
{
    for (const auto& Callback : GameEventCallbacks)
    {
        if (Callback)
        {
            Callback(Event);
        }
    }
}

uint32 Game::GetScore() const
{
    return Score;
}

float Game::GetGameTime() const
{
    return GameTime;
}

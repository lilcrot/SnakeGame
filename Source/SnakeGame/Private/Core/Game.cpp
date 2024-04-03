// Educational project Snake Game
#include "Core/Game.h"
#include "Core/Grid.h"

using namespace SnakeGame;

Game::Game(const FSettings& SettingsIn)  //
    : Settings(SettingsIn)
{
    GameGrid = MakeShared<Grid>(Settings.GridDimension);
}

TSharedPtr<Grid> Game::GetGameGrid() const
{
    return GameGrid;
}

// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/Game.h"

DEFINE_SPEC(FSnakeGame, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FSnakeGame::Define()
{
    using namespace SnakeGame;

    Describe("CoreGame", [this]() {  //
        It("GridShouldExist",
            [this]()
            {
                FSettings GameSettings;
                GameSettings.SnakeConfiguration.StartPosition = SnakeGame::FPosition{
                    GameSettings.GridDimension.Width / 2 + 1, GameSettings.GridDimension.Height / 2 + 1};  // @todo: proper way to handle +1

                const auto CoreGame = MakeUnique<Game>(GameSettings);
                TestTrueExpr(CoreGame->GetGrid().IsValid());
            });
    });
}

#endif

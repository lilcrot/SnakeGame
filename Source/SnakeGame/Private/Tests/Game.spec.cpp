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
        It("GridShouldExists",
            [this]()
            {
                const FSettings GameSettings{10, 10};
                const auto CoreGame = Game(GameSettings);
                TestTrueExpr(CoreGame.GetGameGrid().IsValid());
            });
    });
}

#endif

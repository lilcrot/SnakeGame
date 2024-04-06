// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/Game.h"
#include "Core/Grid.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FSnakeGame, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
TUniquePtr<Game> CoreGame;
END_DEFINE_SPEC(FSnakeGame)

void FSnakeGame::Define()
{
    Describe("Core.Game", [this]() {  //
        BeforeEach(
            [this]()
            {
                FSettings GS;
                GS.GridDimension = FDimension{10, 10};
                GS.SnakeConfiguration.StartPosition = Grid::GetCenter(GS.GridDimension.Width, GS.GridDimension.Height);
                CoreGame = MakeUnique<Game>(GS);
            });

        It("GridMightExist", [this]() { TestTrueExpr(CoreGame->GetGrid().IsValid()); });
        It("SnakeMightExist", [this]() { TestTrueExpr(CoreGame->GetSnake().IsValid()); });
        It("FoodMightExist", [this]() { TestTrueExpr(CoreGame->GetFood().IsValid()); });
    });
}

#endif

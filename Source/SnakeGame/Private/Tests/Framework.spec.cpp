// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/TestUtils.h"
#include "Framework/SG_GameMode.h"
#include "Framework/SG_Pawn.h"

BEGIN_DEFINE_SPEC(FSnakeFramework, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
END_DEFINE_SPEC(FSnakeFramework)

void FSnakeFramework::Define()
{
    using namespace Test;
    Describe("Framework",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap("GameLevel");
                    World = GetTestGameWorld();
                });
            It("GameMapMightExist", [this]() { TestNotNull("World Exists", World); });
            It("ClassesMightBeSetupCorrectly",
                [this]()
                {
                    TestNotNull("Snake game mode set up", Cast<ASG_GameMode>(World->GetAuthGameMode()));

                    const auto FirstPC = World->GetFirstPlayerController();
                    TestNotNull("FirstPlayerController exists", FirstPC);
                    TestNotNull("Snake pawn set up", Cast<ASG_Pawn>(FirstPC->GetPawn()));
                });
            xIt("PawnLocationShouldBeAdjustCorrectly", [this]() { unimplemented(); });
        });
}

#endif

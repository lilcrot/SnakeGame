// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/Food.h"
#include "Core/Types.h"

DEFINE_SPEC(FSnakeFood, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FSnakeFood::Define()
{
    using namespace SnakeGame;

    Describe("Core.Food",
        [this]()
        {
            It("PositionCanBeUpdated",
                [this]()
                {
                    const FPosition Pos = FPosition(45, 67);
                    Food CoreFood;
                    CoreFood.SetPosition(Pos);
                    TestTrueExpr(CoreFood.GetPosition() == Pos);
                });
        });
}

#endif

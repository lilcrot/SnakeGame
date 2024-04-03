// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/Grid.h"

DEFINE_SPEC(FSnakeGrid, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FSnakeGrid::Define()
{
    using namespace SnakeGame;
    Describe("CoreGrid", [this]() {  //
        It("DimsMightIncludeWalls",
            [this]()
            {
                const Grid MyGrid(FDimension{12, 10});
                TestTrueExpr(MyGrid.GetDimension().Width == 14);
                TestTrueExpr(MyGrid.GetDimension().Height == 12);
            });
    });
}

#endif

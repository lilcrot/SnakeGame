// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/Grid.h"
#include "Core/Snake.h"

DEFINE_SPEC(FSnakeGrid, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FSnakeGrid::Define()
{
    using namespace SnakeGame;

    Describe("Core.Grid", [this]() {  //
        It("DimensionShouldIncludeWalls",
            [this]()
            {
                const Grid CoreGrid(FDimension{12, 10});
                TestTrueExpr(CoreGrid.GetDimension().Width == 14);
                TestTrueExpr(CoreGrid.GetDimension().Height == 12);
            });

        It("CenterShouldBeCalculatedCorrectly",  //
            [this]() { TestTrueExpr(Grid::GetCenter(13, 11) == FPosition(7, 6)); });

        It("HitTest.PositionShouldBeUpdatedCorrectly.Food",
            [this]()
            {
                Grid CoreGrid(FDimension{12, 10});
                TestTrueExpr(CoreGrid.HitTest(FPosition(5, 5), ECellGridType::Empty));
                CoreGrid.Update(FPosition(5, 5), ECellGridType::Food);
                TestTrueExpr(CoreGrid.HitTest(FPosition(5, 5), ECellGridType::Food));
            });

        It("HitTest.PositionShouldBeUpdatedCorrectly.Snake",
            [this]()
            {
                const FDimension Dim = FDimension{12, 10};

                FSettings::FSnake Config;
                Config.DefaultSize = 4;
                Config.StartPosition = Grid::GetCenter(Dim.Width, Dim.Height);

                Grid CoreGrid(Dim);
                auto CoreSnake = MakeShared<Snake>(Config);

                TestTrueExpr(CoreGrid.HitTest(FPosition(7, 6), ECellGridType::Empty));
                TestTrueExpr(CoreGrid.HitTest(FPosition(6, 6), ECellGridType::Empty));
                TestTrueExpr(CoreGrid.HitTest(FPosition(5, 6), ECellGridType::Empty));
                TestTrueExpr(CoreGrid.HitTest(FPosition(4, 6), ECellGridType::Empty));

                CoreGrid.Update(CoreSnake->GetLinks().GetHead(), ECellGridType::SnakeBody);

                TestTrueExpr(CoreGrid.HitTest(FPosition(7, 6), ECellGridType::SnakeBody));
                TestTrueExpr(CoreGrid.HitTest(FPosition(6, 6), ECellGridType::SnakeBody));
                TestTrueExpr(CoreGrid.HitTest(FPosition(5, 6), ECellGridType::SnakeBody));
                TestTrueExpr(CoreGrid.HitTest(FPosition(4, 6), ECellGridType::SnakeBody));
            });

        It("HitTest.CellReleaseShouldBeCorrect",
            [this]()
            {
                Grid CoreGrid(FDimension{12, 10});
                CoreGrid.Update(FPosition(5, 5), ECellGridType::Food);
                TestTrueExpr(CoreGrid.HitTest(FPosition(5, 5), ECellGridType::Food));

                CoreGrid.Update(FPosition(5, 6), ECellGridType::Food);
                TestTrueExpr(CoreGrid.HitTest(FPosition(5, 5), ECellGridType::Empty));
                TestTrueExpr(CoreGrid.HitTest(FPosition(5, 6), ECellGridType::Food));
            });

        It("EmptyCellShouldBeFound",
            [this]()
            {
                Grid CoreGrid(FDimension{2, 2});
                TSnakeList Links;
                Links.AddTail(FPosition(1, 1));
                Links.AddTail(FPosition(1, 2));
                Links.AddTail(FPosition(2, 1));
                CoreGrid.Update(Links.GetHead(), ECellGridType::SnakeBody);

                FPosition FoodPosition;
                const bool Founded = CoreGrid.GetRandomEmptyPosition(FoodPosition);
                TestTrueExpr(Founded);
                TestTrueExpr(FoodPosition == FPosition(2, 2));
            });

        // game completed, no empty cells for food generation (rare, almost impossible scenario)
        It("EmptyCellCannotBeFoundWhenSnakeFillGrid",
            [this]()
            {
                Grid CoreGrid(FDimension{2, 2});
                TSnakeList Links;
                Links.AddTail(FPosition(1, 1));
                Links.AddTail(FPosition(1, 2));
                Links.AddTail(FPosition(2, 1));
                Links.AddTail(FPosition(2, 2));
                CoreGrid.Update(Links.GetHead(), ECellGridType::SnakeBody);

                FPosition FoodPosition;
                const bool Founded = CoreGrid.GetRandomEmptyPosition(FoodPosition);
                TestTrueExpr(!Founded);
            });
    });
}

#endif

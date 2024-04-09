// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Utils.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FSnakeGame, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
TUniquePtr<Game> CoreGame;
FSettings GS;
END_DEFINE_SPEC(FSnakeGame)

class MockPositionRandomizer : public IPositionRandomizer
{
public:
    virtual bool GeneratePosition(const FDimension& Dim, const TArray<ECellGridType>& Cells, FPosition& Position) const override
    {
        if (!Positions.IsValidIndex(Index)) return false;

        Position = Positions[Index++];
        return true;
    }

    void SetPositions(const TArray<FPosition>& InPositions)
    {
        Positions = InPositions;
        Index = 0;
    }

private:
    TArray<FPosition> Positions;
    mutable int32 Index = 0;
};

void FSnakeGame::Define()
{
    Describe("Core.Game", [this]() {  //
        BeforeEach(
            [this]()
            {
                GS.GridDimension = FDimension{10, 10};
                GS.SnakeConfiguration.StartPosition = Grid::GetCenter(GS.GridDimension.Width, GS.GridDimension.Height);
                GS.GameSpeed = 1.0f;
                CoreGame = MakeUnique<Game>(GS);
            });

        It("GridMightExist", [this]() { TestTrueExpr(CoreGame->GetGrid().IsValid()); });
        It("SnakeMightExist", [this]() { TestTrueExpr(CoreGame->GetSnake().IsValid()); });
        It("FoodMightExist", [this]() { TestTrueExpr(CoreGame->GetFood().IsValid()); });
        It("GameCanBeOver",
            [this]()
            {
                bool bIsGameOver = false;
                CoreGame->SubscribeOnGameplayEvent(
                    [&bIsGameOver](const EGameplayEvent& Event)
                    {
                        if (Event == EGameplayEvent::GameOver)
                        {
                            bIsGameOver = true;
                        }
                    });

                const int32 Moves = FMath::RoundToInt(GS.GridDimension.Width / 2.0f) - 1;
                for (int32 i = 0; i < Moves; ++i)
                {
                    CoreGame->Update(1.0f, FInput::Default);
                    TestTrueExpr(!bIsGameOver);
                }

                CoreGame->Update(1.0f, FInput::Default);
                TestTrueExpr(bIsGameOver);
            });

    });

    Describe("Core.Game", [this]() {  //
        It("FoodCanBeTaken",
            [this]()
            {
                auto Randomizer = MakeShared<MockPositionRandomizer>();
                Randomizer->SetPositions({FPosition{7, 6}, FPosition{9, 6}, FPosition::Zero});

                GS.GridDimension = FDimension{10, 10};
                GS.SnakeConfiguration.StartPosition = Grid::GetCenter(GS.GridDimension.Width, GS.GridDimension.Height);
                GS.GameSpeed = 1.0f;
                CoreGame = MakeUnique<Game>(GS, Randomizer);

                uint32 Score = 0;
                CoreGame->SubscribeOnGameplayEvent(
                    [&Score](const EGameplayEvent& Event)
                    {
                        if (Event == EGameplayEvent::FoodTaken)
                        {
                            ++Score;
                        }
                    });

                TestTrueExpr(CoreGame->GetScore() == 0);
                TestTrueExpr(Score == 0);

                CoreGame->Update(1.0f, FInput::Default);
                TestTrueExpr(CoreGame->GetScore() == 1);
                TestTrueExpr(Score == 1);

                CoreGame->Update(1.0f, FInput::Default);
                TestTrueExpr(CoreGame->GetScore() == 1);
                TestTrueExpr(Score == 1);

                CoreGame->Update(1.0f, FInput::Default);
                TestTrueExpr(CoreGame->GetScore() == 2);
                TestTrueExpr(Score == 2);
            });
    });
}

#endif

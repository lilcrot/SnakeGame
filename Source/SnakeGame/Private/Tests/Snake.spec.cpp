// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/Snake.h"
#include "Core/Types.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FSnake, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
TUniquePtr<Snake> CoreSnake;
FSettings::FSnake Config;
END_DEFINE_SPEC(FSnake)

void FSnake::Define()
{
    Describe("Core.Snake",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    Config.DefaultSize = 10;
                    Config.StartPosition = FPosition(100, 0);
                    CoreSnake = MakeUnique<Snake>(Config);
                });

            It("MightBeInitCorrectly",
                [this]()
                {
                    FPosition LinkPos = Config.StartPosition;

                    TestTrueExpr(CoreSnake->GetHeadPosition() == LinkPos);
                    TestTrueExpr(CoreSnake->GetLinks().Num() == Config.DefaultSize);

                    auto* Link = CoreSnake->GetLinks().GetHead();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == LinkPos);
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Movement.CanBeMovedRight",
                [this]()
                {
                    FPosition LinkPos = Config.StartPosition;
                    TestTrueExpr(CoreSnake->GetHeadPosition() == LinkPos);
                    CoreSnake->Move(FInput{1, 0});

                    auto* Link = CoreSnake->GetLinks().GetHead();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == FPosition(LinkPos.x + 1, LinkPos.y));
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Movement.CannotBeMovedLeftByDefault",
                [this]()
                {
                    FPosition LinkPos = Config.StartPosition;
                    TestTrueExpr(CoreSnake->GetHeadPosition() == LinkPos);
                    CoreSnake->Move(FInput{-1, 0});

                    auto* Link = CoreSnake->GetLinks().GetHead();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == FPosition(LinkPos.x + 1, LinkPos.y));
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Movement.CanBeMovedUp",
                [this]()
                {
                    FPosition LinkPos = Config.StartPosition;
                    TestTrueExpr(CoreSnake->GetHeadPosition() == LinkPos);
                    CoreSnake->Move(FInput{0, 1});

                    auto* Link = CoreSnake->GetLinks().GetHead();
                    TestTrueExpr(Link->GetValue() == FPosition(LinkPos.x, LinkPos.y + 1));

                    Link = Link->GetNextNode();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == LinkPos);
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Movement.CanBeMovedDown",
                [this]()
                {
                    FPosition LinkPos = Config.StartPosition;
                    TestTrueExpr(CoreSnake->GetHeadPosition() == LinkPos);
                    CoreSnake->Move(FInput{0, -1});

                    auto* Link = CoreSnake->GetLinks().GetHead();
                    TestTrueExpr(Link->GetValue() == FPosition(LinkPos.x, LinkPos.y - 1));

                    Link = Link->GetNextNode();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == LinkPos);
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Size.CanBeIncreased",
                [this]()
                {
                    constexpr uint32 TimeToIncrease = 3;
                    for (uint32 i = 0; i < TimeToIncrease; i++)
                    {
                        CoreSnake->IncreaseLength();
                        TestTrueExpr(CoreSnake->GetLinks().Num() == Config.DefaultSize + i + 1);
                    }
                });
        });
}

#endif

// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/TestUtils.h"
#include "Tests/TestConstants.h"
#include "Framework/SG_GameMode.h"
#include "Framework/SG_Pawn.h"
#include "Misc/PathViews.h"
#include "Framework/SG_GameUserSettings.h"

BEGIN_DEFINE_SPEC(FSnakeFramework, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
USG_GameUserSettings* GameUserSettings;
END_DEFINE_SPEC(FSnakeFramework)

void FSnakeFramework::Define()
{
    using namespace Test;
    using namespace TestConstants;

    Describe("Framework",
        [this]()
        {
            It("GameMapShouldExist",
                [this]()
                {
                    const TArray<FString> SnakeGameMaps = {"M_Game", "M_Menu"};

                    TArray<FString> AllProjectMaps;
                    IFileManager::Get().FindFilesRecursive(AllProjectMaps, *FPaths::ProjectContentDir(), TEXT("*.umap"), true, false);

                    for (const auto& SnakeGameMap : SnakeGameMaps)
                    {
                        const bool SnakeMapExists = AllProjectMaps.ContainsByPredicate(
                            [&](const FString& ProjectMap)
                            {
                                FStringView OutPath, OutName, OutExt;
                                FPathViews::Split(FStringView(ProjectMap), OutPath, OutName, OutExt);
                                return SnakeGameMap.Equals(FString(OutName));
                            });
                        const FString What = SnakeGameMap + FString(" might exist");
                        TestTrue(What, SnakeMapExists);
                    }
                });
        });

    Describe("Framework",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MainGameMapName);
                    World = GetTestGameWorld();
                });
            It("ClassesShouldBeSetupCorrectly",
                [this]()
                {
                    TestNotNull("Snake game mode set up", Cast<ASG_GameMode>(World->GetAuthGameMode()));
                    TestNotNull("Snake pawn set up", Cast<ASG_Pawn>(World->GetFirstPlayerController()->GetPawn()));
                });
            xIt("PawnLocationShouldBeAdjustCorrectly", [this]() { unimplemented(); });
            // TODO: implement
        });

    Describe("Framework.GameUserSettings",
        [this]()
        {
            BeforeEach([this]() { GameUserSettings = NewObject<USG_GameUserSettings>(); });

            It("SpeedOptionsShouldExist",
                [this]()
                {
                    const TArray<FString> TestOptions{"Worm", "Snake", "Python"};
                    const auto SpeedOptions = GameUserSettings->GetGameSpeedOptions();

                    TestTrueExpr(TestOptions.Num() == SpeedOptions.Num());

                    for (const auto& Option : TestOptions)
                    {
                        TestTrueExpr(SpeedOptions.Find(Option) != INDEX_NONE);
                    }
                });

            It("SpeedElemCouldBeFindByName",
                [this]()
                {
                    const TArray<TTuple<FString, EGameSpeed>> TestData{
                        {"Worm", EGameSpeed::Worm}, {"Snake", EGameSpeed::Snake}, {"Python", EGameSpeed::Python}};

                    for (const auto& [Name, Speed] : TestData)
                    {
                        const FString What = FString::Printf(TEXT("%s name not compliant with [%i] of the EGameSpeed"), *Name, Speed);
                        TestTrue(What, Speed == GameUserSettings->GetGameSpeedByName(Name));
                    }
                });

            It("SpeedElemShouldBeDefaultIfNameDoesntExist",
                [this]() { TestTrueExpr(EGameSpeed::Snake == GameUserSettings->GetGameSpeedByName("xxxxxxxxxxxxxxxxxxxxx")); });

            It("SettingsCanBeSaved",
                [this]()
                {
                    const SnakeGame::FDimension PrevDimension = GameUserSettings->GetGridSize();
                    EGridSize PrevGridSize;

                    const float PrevSpeed = GameUserSettings->GetGameSpeed();
                    EGameSpeed PrevGameSpeed;

                    /* Grid settings */
                    {
                        const TArray<TTuple<FString, EGridSize, SnakeGame::FDimension>> TestData{
                            {"30x10", EGridSize::Size_30x10, SnakeGame::FDimension{30, 10}},
                            {"50x15", EGridSize::Size_50x15, SnakeGame::FDimension{50, 15}},
                            {"80x20", EGridSize::Size_80x20, SnakeGame::FDimension{80, 20}}};

                        for (const auto& [Name, GridSizeType, GridSize] : TestData)
                        {
                            if (GridSize == PrevDimension)
                            {
                                PrevGridSize = GridSizeType;
                            }
                        }

                        for (const auto& [Name, GridSizeType, GridSize] : TestData)
                        {
                            GameUserSettings->SaveSnakeSettings(EGameSpeed::Snake, GridSizeType);

                            TestTrueExpr(GameUserSettings->GetCurrentGridSizeOption().Equals(Name));
                            TestTrueExpr(GameUserSettings->GetGridSize().Width == GridSize.Width);
                            TestTrueExpr(GameUserSettings->GetGridSize().Height == GridSize.Height);
                        }
                    }

                    /* Speed settings */
                    {
                        const TArray<TTuple<FString, EGameSpeed, float>> TestData{
                            {"Worm", EGameSpeed::Worm, 0.3f}, {"Snake", EGameSpeed::Snake, 0.1f}, {"Python", EGameSpeed::Python, 0.05f}};

                        for (const auto& [Name, SpeedType, Speed] : TestData)
                        {
                            if (Speed == PrevSpeed)
                            {
                                PrevGameSpeed = SpeedType;
                            }
                        }

                        for (const auto& [Name, SpeedType, Speed] : TestData)
                        {
                            GameUserSettings->SaveSnakeSettings(SpeedType, EGridSize::Size_30x10);
                            TestTrueExpr(GameUserSettings->GetCurrentGameSpeedOption().Equals(Name));
                            TestTrueExpr(GameUserSettings->GetGameSpeed() == Speed);
                        }
                    }

                    // return to prev settings
                    GameUserSettings->SaveSnakeSettings(PrevGameSpeed, PrevGridSize);
                });

            It("GridSizeOptionsShouldExist",
                [this]()
                {
                    const TArray<FString> TestOptions{"30x10", "50x15", "80x20"};
                    const auto GridOptions = GameUserSettings->GetGridSizeOptions();

                    TestTrueExpr(TestOptions.Num() == GridOptions.Num());

                    for (const auto& Option : TestOptions)
                    {
                        TestTrueExpr(GridOptions.Find(Option) != INDEX_NONE);
                    }
                });

            It("GridSizeElemCouldBeFindByName",
                [this]()
                {
                    const TArray<TTuple<FString, EGridSize>> TestData{
                        {"30x10", EGridSize::Size_30x10}, {"50x15", EGridSize::Size_50x15}, {"80x20", EGridSize::Size_80x20}};

                    for (const auto& [Name, GridSize] : TestData)
                    {
                        const FString What = FString::Printf(TEXT("%s name not compliant with [%i] of the EGridSize"), *Name, GridSize);
                        TestTrue(What, GridSize == GameUserSettings->GetGridSizeByName(Name));
                    }
                });

            It("GridSizeElemShouldBeDefaultIfNameDoesntExist",
                [this]() { TestTrueExpr(EGridSize::Size_50x15 == GameUserSettings->GetGridSizeByName("xxxxxxxxxxxxxxxxxxxxx")); });
        });
}

#endif

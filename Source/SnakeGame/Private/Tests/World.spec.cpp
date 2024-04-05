// Educational project Snake Game

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/TestUtils.h"
#include "Tests/TestConstants.h"
#include "World/SG_Grid.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Core/Grid.h"
#include "World/SG_WorldTypes.h"

BEGIN_DEFINE_SPEC(FSnakeWorld, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
SnakeGame::FDimension Dim;
uint32 CellSize;
TSharedPtr<SnakeGame::Grid> ModelGrid;
UStaticMeshComponent* GridStaticMesh;
ASG_Grid* GridVisual;
END_DEFINE_SPEC(FSnakeWorld)

void FSnakeWorld::Define()
{
    using namespace Test;
    using namespace TestConstants;

    Describe("WorldGrid",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(TestEmptyMapName);
                    World = GetTestGameWorld();

                    constexpr char* GridBPName = "Blueprint'/Game/World/BP_SnakeGrid.BP_SnakeGrid'";

                    Dim = SnakeGame::FDimension{10, 10};
                    CellSize = 20;
                    ModelGrid = MakeShared<SnakeGame::Grid>(Dim);

                    const FTransform Origin = FTransform::Identity;
                    GridVisual = CreateBlueprintDeferred<ASG_Grid>(World, GridBPName, Origin);
                    GridVisual->SetModel(ModelGrid, CellSize);
                    GridVisual->FinishSpawning(Origin);

                    auto* Comp = GridVisual->GetComponentByClass(UStaticMeshComponent::StaticClass());
                    GridStaticMesh = Cast<UStaticMeshComponent>(Comp);
                });

            It("StaticGridMightHaveCorrectTransform",
                [this]()
                {
                    const FBox Box = GridStaticMesh->GetStaticMesh()->GetBoundingBox();
                    const auto Size = Box.GetSize();

                    const auto WorldWidth = ModelGrid->GetDimension().Width * CellSize;
                    const auto WorldHeight = ModelGrid->GetDimension().Height * CellSize;

                    TestTrueExpr(GridStaticMesh->GetRelativeLocation().Equals(0.5 * FVector(WorldHeight, WorldWidth, -Size.Z)));
                    TestTrueExpr(GridStaticMesh->GetRelativeScale3D().Equals(FVector(WorldHeight / Size.X, WorldWidth / Size.Y, 1.0)));
                });

            It("ColorsMightBeSetupCorrectly",
                [this]()
                {
                    FSnakeColors Colors;
                    Colors.GridBackgroundColor = FLinearColor::Green;
                    Colors.GridWallColor = FLinearColor::Red;
                    Colors.GridLineColor = FLinearColor::Yellow;
                    GridVisual->UpdateColors(Colors);

                    auto* Material = GridStaticMesh->GetMaterial(0);
                    FLinearColor ColorToCheck;

                    Material->GetVectorParameterValue(FName("BackgroundColor"), ColorToCheck);
                    TestTrueExpr(ColorToCheck.Equals(Colors.GridBackgroundColor));

                    Material->GetVectorParameterValue(FName("WallColor"), ColorToCheck);
                    TestTrueExpr(ColorToCheck.Equals(Colors.GridWallColor));

                    Material->GetVectorParameterValue(FName("LineColor"), ColorToCheck);
                    TestTrueExpr(ColorToCheck.Equals(Colors.GridLineColor));
                });
        });

    Describe("WorldGrid",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MainGameMapName);
                    World = GetTestGameWorld();
                });
            It("OnlyOneValidGridActorShouldExist",
                [this]()
                {
                    TArray<AActor*> Grids;
                    UGameplayStatics::GetAllActorsOfClass(World, ASG_Grid::StaticClass(), Grids);

                    const bool bHaveOnlyOneGrid = Grids.Num() == 1;
                    TestTrueExpr(bHaveOnlyOneGrid);

                    if (bHaveOnlyOneGrid)
                    {
                        TestNotNull("Grid actor exists", Grids[0]);
                    }
                });
        });
}

#endif

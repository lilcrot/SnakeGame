// Educational project Snake Game
#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldGrid, All, All);

ASG_Grid::ASG_Grid()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    GridMesh = CreateDefaultSubobject<UStaticMeshComponent>("GridMesh");
    check(GridMesh);
    GridMesh->SetupAttachment(Origin);
}

void ASG_Grid::SetModel(const TSharedPtr<SnakeGame::Grid>& Grid, const uint32 InCellSize)
{
    if (!Grid.IsValid())
    {
        UE_LOG(LogWorldGrid, Fatal, TEXT("Grid is null, game aborted!"));
    }

    GridDimension = Grid.Get()->GetDimension();
    CellSize = InCellSize;
    WorldWidth = GridDimension.Width * CellSize;
    WorldHeight = GridDimension.Height * CellSize;

    /* scale mesh */
    {
        const auto GridStaticMesh = GridMesh->GetStaticMesh();
        checkf(GridStaticMesh, TEXT("GridStaticMesh doesn't exist"));
        const FBox Box = GridStaticMesh->GetBoundingBox();
        const FVector Size = Box.GetSize();

        check(Size.X);
        check(Size.Y);
        GridMesh->SetRelativeScale3D(FVector(WorldHeight / Size.X, WorldWidth / Size.Y, 1.0));
        GridMesh->SetRelativeLocation(0.5 * FVector(WorldHeight, WorldWidth, -Size.Z));
    }
    /* setup material */
    {
        GridMaterial = GridMesh->CreateAndSetMaterialInstanceDynamic(0);
        if (GridMaterial)
        {
            GridMaterial->SetVectorParameterValue("Division", FVector(GridDimension.Height, GridDimension.Width, 0.0));
        }
    }
}

void ASG_Grid::UpdateColors(const FSnakeColors& Colors)
{
    if (GridMaterial)
    {
        GridMaterial->SetVectorParameterValue("BackgroundColor", Colors.GridBackgroundColor);
        GridMaterial->SetVectorParameterValue("LineColor", Colors.GridLineColor);
        GridMaterial->SetVectorParameterValue("WallColor", Colors.GridWallColor);
    }
}

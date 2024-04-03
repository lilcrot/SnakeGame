// Educational project Snake Game
#include "Framework/SG_Pawn.h"
#include "Camera/CameraComponent.h"

namespace
{
float HalfFOVTan(const float FOVDegrees)
{
    return FMath::Tan(FMath::DegreesToRadians(FOVDegrees * 0.5f));
}

float VerticalFOV(const float HorFOVDegrees, const float ViewportAspectHW)
{
    // https://en.m.wikipedia.org/wiki/Field_of_view_in_video_games
    return FMath::RadiansToDegrees(2.0f * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HorFOVDegrees) * 0.5f) * ViewportAspectHW));
}

}  // namespace

ASG_Pawn::ASG_Pawn()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    check(Camera);
    Camera->SetupAttachment(Origin);
}

void ASG_Pawn::UpdateLocation(const SnakeGame::FDimension& InDim, const int32 InCellSize, const FTransform& InGridOrigin)
{
    Dim = InDim;
    CellSize = InCellSize;
    GridOrigin = InGridOrigin;

    check(GEngine);
    check(GEngine->GameViewport);
    check(GEngine->GameViewport->Viewport);

    auto* Viewport = GEngine->GameViewport->Viewport;
    Viewport->ViewportResizedEvent.AddUObject(this, &ASG_Pawn::OnViewportResized);

#if WITH_EDITOR
    OnViewportResized(Viewport, 0);
#endif
}

void ASG_Pawn::OnViewportResized(FViewport* Viewport, uint32 Val)
{
    if (!Viewport || Viewport->GetSizeXY().Y == 0 || Dim.Height == 0) return;

    const float WorldWidth = Dim.Width * CellSize;
    const float WorldHeight = Dim.Height * CellSize;

    float LocationZ = 0.0f;
    const float ViewportAspect = static_cast<float>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const float GridAspect = static_cast<float>(Dim.Width) / Dim.Height;

    if (ViewportAspect <= GridAspect)
    {
        LocationZ = WorldWidth / HalfFOVTan(Camera->FieldOfView);
    }
    else
    {
        check(ViewportAspect);
        const float VFOV = VerticalFOV(Camera->FieldOfView, 1.0f / ViewportAspect);
        LocationZ = WorldHeight / HalfFOVTan(VFOV);
    }

    const FVector NewPawnLocation = GridOrigin.GetLocation() + 0.5f * FVector(WorldHeight, WorldWidth, LocationZ);
    SetActorLocation(NewPawnLocation);
}
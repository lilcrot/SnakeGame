// Educational project Snake Game
#include "Framework/SG_Pawn.h"
#include "Camera/CameraComponent.h"

namespace
{
double HalfFOVTan(const double FOVDegrees)
{
    return FMath::Tan(FMath::DegreesToRadians(FOVDegrees * 0.5));
}

double VerticalFOV(const double HorFOVDegrees, const double ViewportAspectHW)
{
    // https://en.m.wikipedia.org/wiki/Field_of_view_in_video_games
    return FMath::RadiansToDegrees(2.0 * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HorFOVDegrees) * 0.5) * ViewportAspectHW));
}
constexpr double GridMargin = 2.0;

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
    Viewport->ViewportResizedEvent.Remove(ResizeHandle);
    ResizeHandle = Viewport->ViewportResizedEvent.AddUObject(this, &ASG_Pawn::OnViewportResized);

    OnViewportResized(Viewport, 0);
}

void ASG_Pawn::OnViewportResized(FViewport* Viewport, uint32 Val)
{
    if (!Viewport || Viewport->GetSizeXY().Y == 0 || Dim.Height == 0) return;

    const double WorldWidth = Dim.Width * CellSize;
    const double WorldHeight = Dim.Height * CellSize;

    double LocationZ = 0.0;
    const double ViewportAspect = static_cast<double>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const double GridAspect = static_cast<double>(Dim.Width) / Dim.Height;

    if (ViewportAspect <= GridAspect)
    {
        const double MarginWidth = (Dim.Width + GridMargin) * CellSize;
        LocationZ = MarginWidth / HalfFOVTan(Camera->FieldOfView);
    }
    else
    {
        check(ViewportAspect);

        const double VFOV = VerticalFOV(Camera->FieldOfView, 1.0 / ViewportAspect);
        const double MarginHeight = (Dim.Height + GridMargin) * CellSize;
        LocationZ = MarginHeight / HalfFOVTan(VFOV);
    }

    const FVector NewPawnLocation = GridOrigin.GetLocation() + 0.5 * FVector(WorldHeight, WorldWidth, LocationZ);
    SetActorLocation(NewPawnLocation);
}
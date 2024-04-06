#pragma once

#include "Core/Types.h"
#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

namespace SnakeGame
{
class WorldUtils
{
public:
    static FVector PositionToVector(const SnakeGame::FPosition& Position, const uint32 CellSize, const SnakeGame::FDimension& Dim)
    {
        return FVector((Dim.Height - 1 - Position.y) * CellSize, Position.x * CellSize, 0.0) + FVector(CellSize * 0.5);
    }

    static void ScaleMesh(UStaticMeshComponent* Mesh, const FVector& WorldSize)
    {
        if (Mesh && Mesh->GetStaticMesh())
        {
            const FBox Box = Mesh->GetStaticMesh()->GetBoundingBox();
            const FVector Size = Box.GetSize();

            check(!Size.IsZero());
            Mesh->SetRelativeScale3D(FVector(WorldSize / Size));
        }
    }
};
}  // namespace SnakeGame

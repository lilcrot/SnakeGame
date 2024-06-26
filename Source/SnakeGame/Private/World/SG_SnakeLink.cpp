// Educational project Snake Game

#include "World/SG_SnakeLink.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASG_SnakeLink::ASG_SnakeLink()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    checkf(Origin, TEXT("Origin is failed to create!"));
    SetRootComponent(Origin);

    LinkMesh = CreateDefaultSubobject<UStaticMeshComponent>("LinkMesh");
    checkf(LinkMesh, TEXT("LinkMesh is failed to create!"));
    LinkMesh->SetupAttachment(Origin);
}

void ASG_SnakeLink::UpdateColor(const FLinearColor& Color)
{
    LinkColor = Color;
    if (auto* LinkMaterial = LinkMesh->CreateAndSetMaterialInstanceDynamic(0))
    {
        LinkMaterial->SetVectorParameterValue("Color", Color);
    }
}

void ASG_SnakeLink::UpdateScale(const uint32 CellSize)
{
    checkf(LinkMesh->GetStaticMesh(), TEXT("StaticMesh isn't set for LinkMesh"));
    const FBox Box = LinkMesh->GetStaticMesh()->GetBoundingBox();
    const auto Size = Box.GetSize();

    check(Size.X);
    check(Size.Y);
    LinkMesh->SetRelativeScale3D(FVector(CellSize / Size.X, CellSize / Size.Y, CellSize / Size.Z));
}

void ASG_SnakeLink::PlayExplodeEffect()
{
    auto* NiagaraSystem = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
    if (NiagaraSystem)
    {
        NiagaraSystem->SetNiagaraVariableLinearColor("Color", LinkColor);
    }
    SetActorHiddenInGame(true);
}

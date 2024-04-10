// Educational project Snake Game
#include "UI/SG_MenuHUD.h"
#include "UI/SG_StartGameWidget.h"

void ASG_MenuHUD::BeginPlay()
{
    Super::BeginPlay();

    StartGameWidget = CreateWidget<USG_StartGameWidget>(GetWorld(), StartGameWidgetClass);
    checkf(StartGameWidget, TEXT("StartGameWidget is failed to create"));
    StartGameWidget->AddToViewport();
}

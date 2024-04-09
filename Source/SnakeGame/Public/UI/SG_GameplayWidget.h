// Educational project Snake Game
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SG_GameplayWidget.generated.h"

class UTextBlock;

UCLASS(Abstract)
class SNAKEGAME_API USG_GameplayWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetGameTime(float Seconds);
    void SetScore(const uint32 Score);
    void SetResetGameKeyName(const FString& ResetGameKeyName);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TimeText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ScoreText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ResetGameText;
};

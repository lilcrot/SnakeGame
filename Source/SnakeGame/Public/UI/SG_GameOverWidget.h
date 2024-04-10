// Educational project Snake Game
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SG_GameOverWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS(Abstract)
class SNAKEGAME_API USG_GameOverWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetScore(const uint32 Score);
    void SetResetGameKeyName(const FString& ResetGameKeyName);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ScoreText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ResetGameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> BackToMenuButton;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> MenuLevel;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnBackToMenu();
};

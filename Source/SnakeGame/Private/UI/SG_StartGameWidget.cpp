// Educational project Snake Game
#include "UI/SG_StartGameWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Framework/SG_GameUserSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogStartGameWidget, All, All);

void USG_StartGameWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto* UserSettings = USG_GameUserSettings::Get();
    if (!UserSettings)
    {
        UE_LOG(LogStartGameWidget, Error, TEXT("SG_GameUserSettings isn't valid so NativeOnInitialized is failed"));
        return;
    }

    checkf(StartGameButton, TEXT("StartGameButton doesn't exist"));
    StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);

    checkf(CloseGameButton, TEXT("CloseGameButton doesn't exist"));
    CloseGameButton->OnClicked.AddDynamic(this, &ThisClass::OnCloseGame);

    InitComboBox(
        GameSpeedComboBox,                                             //
        [&]() { return UserSettings->GetGameSpeedOptions(); },         //
        [&]() { return UserSettings->GetCurrentGameSpeedOption(); });  //

    InitComboBox(
        GridSizeComboBox,                                             //
        [&]() { return UserSettings->GetGridSizeOptions(); },         //
        [&]() { return UserSettings->GetCurrentGridSizeOption(); });  //
}

void USG_StartGameWidget::InitComboBox(const TObjectPtr<UComboBoxString>& ComboBox,  //
    TFunction<TArray<FString>()> OptionsGetter, TFunction<FString()> CurrentOptionGetter)
{
    checkf(ComboBox, TEXT("ComboBox in InitComboBox func isn't valid"));
    ComboBox->ClearOptions();

    for (const auto& Option : OptionsGetter())
    {
        ComboBox->AddOption(Option);
    }
    ComboBox->SetSelectedOption(CurrentOptionGetter());
    ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnSelectionChanged);
}

void USG_StartGameWidget::OnStartGame()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameLevel);
}

void USG_StartGameWidget::OnCloseGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void USG_StartGameWidget::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::OnMouseClick)
    {
        SaveSettings();
    }
}

void USG_StartGameWidget::SaveSettings()
{
    if (auto* UserSettings = USG_GameUserSettings::Get())
    {
        const EGameSpeed GameSpeed = UserSettings->GetGameSpeedByName(GameSpeedComboBox->GetSelectedOption());
        const EGridSize GridSize = UserSettings->GetGridSizeByName(GridSizeComboBox->GetSelectedOption());
        UserSettings->SaveSnakeSettings(GameSpeed, GridSize);
    }
}
// Educational project Snake Game
#include "Framework/SG_GameUserSettings.h"
#include "Core/Types.h"

USG_GameUserSettings::USG_GameUserSettings()
{
    check(GameSpeedsMap.Num() == StaticEnum<EGameSpeed>()->NumEnums() - 1);
    check(GridSizesMap.Num() == StaticEnum<EGridSize>()->NumEnums() - 1);
}

USG_GameUserSettings* USG_GameUserSettings::Get()
{
    return GEngine ? Cast<USG_GameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

TArray<FString> USG_GameUserSettings::GetGameSpeedOptions() const
{
    return GetOptionNames(GameSpeedsMap);
}
FString USG_GameUserSettings::GetCurrentGameSpeedOption() const
{
    return CurrentSpeed.Name;
}

TArray<FString> USG_GameUserSettings::GetGridSizeOptions() const
{
    return GetOptionNames(GridSizesMap);
}
FString USG_GameUserSettings::GetCurrentGridSizeOption() const
{
    return CurrentGridSize.Name;
}

void USG_GameUserSettings::SaveSnakeSettings(const EGameSpeed& GameSpeed, const EGridSize& GridSize)
{
    if (!GameSpeedsMap.Contains(GameSpeed) || !GridSizesMap.Contains(GridSize)) return;

    CurrentSpeed = GameSpeedsMap[GameSpeed];
    CurrentGridSize = GridSizesMap[GridSize];
}

EGameSpeed USG_GameUserSettings::GetGameSpeedByName(const FString& Name) const
{
    return FindOptionByNameOrDefault(GameSpeedsMap, Name, EGameSpeed::Snake);
}

EGridSize USG_GameUserSettings::GetGridSizeByName(const FString& Name) const
{
    return FindOptionByNameOrDefault(GridSizesMap, Name, EGridSize::Size_50x15);
}

float USG_GameUserSettings::GetGameSpeed() const
{
    return CurrentSpeed.Value;
}

SnakeGame::FDimension USG_GameUserSettings::GetGridSize() const
{
    return CurrentGridSize.Value;
}
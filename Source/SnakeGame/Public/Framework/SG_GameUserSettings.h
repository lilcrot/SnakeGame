// Educational project Snake Game
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/Types.h"

#include "SG_GameUserSettings.generated.h"

UENUM()
enum class EGameSpeed : uint8
{
    Worm = 0,
    Snake, /* default */
    Python
};

UENUM()
enum class EGridSize : uint8
{
    Size_30x10 = 0,
    Size_50x15, /* default */
    Size_80x20
};

USTRUCT()
struct FSpeedData
{
    GENERATED_BODY()

    FString Name;
    float Value;
};

USTRUCT()
struct FGridData
{
    GENERATED_BODY()

    FString Name;
    SnakeGame::FDimension Value;
};

UCLASS()
class SNAKEGAME_API USG_GameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    USG_GameUserSettings();

    static USG_GameUserSettings* Get();

    TArray<FString> GetGameSpeedOptions() const;
    FString GetCurrentGameSpeedOption() const;

    TArray<FString> GetGridSizeOptions() const;
    FString GetCurrentGridSizeOption() const;

    void SaveSnakeSettings(const EGameSpeed& GameSpeed, const EGridSize& GridSize);

    EGameSpeed GetGameSpeedByName(const FString& Name) const;
    EGridSize GetGridSizeByName(const FString& Name) const;

    float GetGameSpeed() const;
    SnakeGame::FDimension GetGridSize() const;

private:
    const TMap<EGameSpeed, FSpeedData> GameSpeedsMap  //
        {
            {EGameSpeed::Worm, {"Worm", 0.3f}},       //
            {EGameSpeed::Snake, {"Snake", 0.1f}},     //
            {EGameSpeed::Python, {"Python", 0.05f}},  //
        };

    const TMap<EGridSize, FGridData> GridSizesMap  //
        {
            {EGridSize::Size_30x10, {"30x10", SnakeGame::FDimension{30, 10}}},  //
            {EGridSize::Size_50x15, {"50x15", SnakeGame::FDimension{50, 15}}},  //
            {EGridSize::Size_80x20, {"80x20", SnakeGame::FDimension{80, 20}}},  //
        };

    FSpeedData CurrentSpeed = GameSpeedsMap[EGameSpeed::Snake];
    FGridData CurrentGridSize = GridSizesMap[EGridSize::Size_50x15];

    template <typename MapType, typename EnumType>
    EnumType FindOptionByNameOrDefault(const MapType& Map, const FString& Name, EnumType Default) const;

    template <typename MapType>
    TArray<FString> GetOptionNames(const MapType& Map) const;
};

template <typename MapType, typename EnumType>
inline EnumType USG_GameUserSettings::FindOptionByNameOrDefault(const MapType& Map, const FString& Name, EnumType Default) const
{
    const auto* Founded = Algo::FindByPredicate(Map, [Name](const auto& Pair) { return Pair.Value.Name.Equals(Name); });
    return Founded ? Founded->Key : Default;
}

template <typename MapType>
inline TArray<FString> USG_GameUserSettings::GetOptionNames(const MapType& Map) const
{
    TArray<FString> Names;
    Algo::Transform(Map, Names, [](const auto& Pair) { return Pair.Value.Name; });
    return Names;
}
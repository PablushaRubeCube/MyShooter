#pragma once

#include "ShooterCoreTypes.generated.h"

//item
UENUM(BlueprintType)
enum class EItemRare : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX
};

UENUM(BlueprintType)
enum class EItemStates : uint8
{
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Pickup UMETA(DispayName = "Pickup"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Falling UMETA(DisplayName = "Falling"),

	EIS_MAX

};

USTRUCT(BlueprintType)
struct FGlowMaterial
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UCurveVector* MaterialCurve;

	UPROPERTY(EditAnywhere)
		UCurveVector* InterpItemCurve;

	FTimerHandle TimerGlowMaterial;

	UPROPERTY(EditAnywhere)
		float TimeGlowMaterial = 5.f;

	UPROPERTY(EditAnywhere)
		float FresnelMultiplay = 150.f;

	UPROPERTY(EditAnywhere)
		float ExponentFresnel = 3.f;

	UPROPERTY(EditAnywhere)
		float FresnelFraction = 4.f;
};

//weapon
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_9MM UMETA(DisplayName = "9mm"),
	EAT_AR UMETA(DisplayName = "AR"),

	EAT_MAX
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_SubmachineGun UMETA(DisplayName = "SMG"),
	EWT_AssaultRifle UMETA(DisplayName = "Rifle"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),

	EWT_MAX
};
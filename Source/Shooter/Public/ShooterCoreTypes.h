#pragma once

#include "Engine/DataTable.h"
#include "ShooterCoreTypes.generated.h"


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

USTRUCT(BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity")
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity")
	FLinearColor TopWidgetColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity")
	FLinearColor DownWidgetColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity")
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity")
	UTexture2D* IconBackground;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity")
	int32 CustomDepthStencil;
};

USTRUCT(BlueprintType)
struct FWeaponPropertiesTable : public FTableRowBase
{
	GENERATED_BODY()

		/**Store Ammo Value current Weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	int32 Ammo;
	/** Store Max Magazine Capacity*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	int32 MagazineCapacity;

	/**Store type of Weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	EWeaponType WeaponType;

	/**Store Type of use Ammo*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	class  USkeletalMesh* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperty")
	UTexture2D* AmmoIcon;
};

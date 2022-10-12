// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterCoreTypes.h"
#include "Item.generated.h"

UCLASS()
class SHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

private:// variables

	//Count Stars
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;

	//StateItem
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemStates ItemStates = EItemStates::EIS_Pickup;

	//store make in editor curve for pickup item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Curve", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Curve", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemScaleCurve = nullptr;

	//Store Timer for curve function
	FTimerHandle TimerInterpCurve;

	//Time how long we will be interp Item
	float InterpTimeCurve = 0.7f;

	//true when we start pickup item
	bool IsBeTaken = false;

	//store intin location when we pickup item
	FVector ItemStartCurveLocation = FVector::ZeroVector;

	//store player camera location
	FVector CameraPlayerLocation = FVector::ZeroVector;

	UPROPERTY()
	class AShooterCharacter* Character = nullptr;

	// variable for store initial rotation item when we start pickup interp
	float InterpInitialYawOffset = 0.f;

private://functions

	void PlayPickupSound(AShooterCharacter* Char);

	void StartTimerGlowMaterial();

	void UpdateGlowMaterial();

	void SetDepthStencilValue();

protected://variables

	//Collides with the box to show widget 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Properties")
	class UBoxComponent* CollisionBox;

	//widget for show info about item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Properties")
	class UWidgetComponent* PickUpWidget;

	//Sphere component to limit area where we can widget toogle 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Base Properties")
	class USphereComponent* AgroSphere;

	//Rare Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity", meta = (AllowPrivateAccess = "true"))
	EItemRare ItemRare = EItemRare::EIR_MAX;

	//Store Glow material for gun
	UPROPERTY()
	UMaterialInstanceDynamic* MeshMaterialDynamic;

	UPROPERTY(EditAnywhere, Category = "Material")
	FGlowMaterial GlowMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Rarity")
	UDataTable* RarityTableData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rarity")
	FItemRarityTable RarityRow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FItemPropertiesTable ItemPropertiesRow;

protected://functions
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	//Overlap agro Function
	UFUNCTION()
		void BeginOverlapAgroSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndOverlapAgroSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//change ActiveStars depending on ItemRare
	void SetRare();

	//Set properties item 
	void SetItemProperties(EItemStates State);

	//Call when TimerCurve finish //call when TimerCurve end / stop interp movement to camera char
	void FinishInterpItem();

	// Call if IsBeTaken true. after that start interp movement item
	void ItemInterp(float DeltaTime);

	void ToggleCustomDepth(bool bEnableCustomDepth);

	void ToggleGlowMaterial(bool bEnableGlowMaterial);

	void SetColorFresnel();

	void SetRarity();

	//Ovveride Mesh in Child. Weapon -> SkeletalMeshComponent, Ammo -> StaticMeshComponent
	virtual UMeshComponent* GetMeshComponent() PURE_VIRTUAL(AItem::GetMeshComponent, return nullptr;);

	//Get Sounds
	virtual class USoundCue* GetPickupSound() { return ItemPropertiesRow.PickupSound; }
	virtual USoundCue* GetEquipSound() { return ItemPropertiesRow.EquipSound; }

	//Material
	UMaterialInstance* GetMeshMaterial() { return ItemPropertiesRow.MeshMaterial; };
	int32 GetMaterialIndex() { return ItemPropertiesRow.MaterialIndex; };
public://functions

	virtual void Tick(float DeltaTime) override;

	//Return PickUpWidget/CollisionBox/AgroSphere
	FORCEINLINE UWidgetComponent* GetPickUpWidget() const { return PickUpWidget; }
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE USphereComponent* GetAgroSphere() const { return  AgroSphere; }

	//Set/get for State Item
	FORCEINLINE EItemStates GetItemStates() const { return ItemStates; }
	void SetItemStates(EItemStates State);

	//Char call when start pickup item
	virtual	void StartPickupItem(AShooterCharacter* Char);

	virtual void GetPickupItem();

	FORCEINLINE EItemRare GetItemRare() const { return ItemRare; }
};

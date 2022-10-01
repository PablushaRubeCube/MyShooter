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

	//Name of specifically item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName = FString("Default");

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

protected://variables

	//Skeletal Mesh of item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemMesh;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemRare ItemRare = EItemRare::EIR_MAX;

	//Amount our ammo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 AmmoAmount = 0.f;

	/** Sound pickup current item */
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundCue* PickupSound;

	/** Sound Equip current item */
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* EquipSound;
	
	/** Set Pickup widget Icon*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	UTexture2D* WidgetIcon;

	/** Set Material for the Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInstance* MeshMaterial;

	/** Store Material index for dynamic changes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	int32 MaterialIndex = 0;

	//Store Glow material for gun
	UPROPERTY()
	UMaterialInstanceDynamic* MeshMaterialDynamic;

	UPROPERTY(EditAnywhere, Category = "Material")
	FGlowMaterial GlowMaterial;

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

	virtual UMeshComponent* GetMeshComponent() const { return ItemMesh; }

	void ToggleCustomDepth(bool bEnableCustomDepth);

	void ToggleGlowMaterial(bool bEnableGlowMaterial);

public://functions

	virtual void Tick(float DeltaTime) override;

	//Return PickUpWidget/CollisionBox/AgroSphere
	FORCEINLINE UWidgetComponent* GetPickUpWidget() const { return PickUpWidget; }
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE USphereComponent* GetAgroSphere() const { return  AgroSphere; }

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return ItemMesh; }

	//Set/get for State Item
	FORCEINLINE EItemStates GetItemStates() const { return ItemStates; }
	void SetItemStates(EItemStates State);

	//Char call when start pickup item
	virtual	void StartPickupItem(AShooterCharacter* Char);

	//Get Sounds
	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }

	FORCEINLINE int32 GetAmmoAmount() const { return AmmoAmount; }

	virtual void GetPickupItem();

	FORCEINLINE EItemRare GetItemRare() const { return ItemRare; }
};

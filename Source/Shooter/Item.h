// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRare : uint8
{
	EIR_Damaged UMETA(DisplayName "Damaged"),
	EIR_Common UMETA(DisplayName "Common"),
	EIR_Uncommon UMETA(DisplayName "Incommon"),
	EIR_Rare UMETA(DisplayName "Rare"),
	EIR_Legendary UMETA(DisplayName "Legendary"),

	EIR_Max UMETA(DisplayName "Default")
};

UENUM(BlueprintType)
enum class EItemStates : uint8
{
	EIS_Equipped UMETA(DisplayName"Equipped"),
	EIS_Pickup UMETA(DispayName "Pickup"),
	EIS_EquipInterping UMETA(DisplayName"EquipInterping"),
	EIS_PickedUp UMETA (DisplayName "PickedUp"),
	EIS_Falling UMETA(DisplayName"Falling"),

	EIS_Max UMETA(DispalyName "Default")

};


UCLASS()
class SHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//Skeletal Mesh of item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemMesh;

	//Collides with the box to show widget 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	//widget for show info about item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickUpWidget;

	//Sphere component to limit area where we can widget toogle 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AgroSphere;

	//Name of specifically item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	//Amount our ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 Amount;

	//Rare Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemRare ItemRare;

	//Count Stars
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;

	//StateItem
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemStates ItemStates;

	//store make in editor curve for pickup item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Curve", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Curve", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemScaleCurve;

	//Store Timer for curve function
	FTimerHandle TimerCurve;

	//Time how long we will be interp Item
	float InterpTimeCurve;
	
	//true when we start pickup item
	bool IsBeTaken;

	//store intin location when we pickup item
	FVector ItemStartCurveLocation;

	//store player camera location
	FVector CameraPlayerLocation;

	UPROPERTY()
	class AShooterCharacter* Character;

	// variable for store initial rotation item when we start pickup interp
	float InterpInitialYawOffset;

public:
	//Return PickUpWidget/CollisionBox/AgroSphere
	FORCEINLINE UWidgetComponent* GetPickUpWidget() const { return PickUpWidget; }
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE USphereComponent* GetAgroSphere() const { return  AgroSphere; }

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return ItemMesh; }
	//Set/get for State Item
	FORCEINLINE EItemStates GetItemStates() const { return ItemStates; }
	void SetItemStates(EItemStates State);

	//Char call when start pickup item
	void StartCurveItem(AShooterCharacter* Char);
};

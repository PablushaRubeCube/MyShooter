// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PickupComponent.h"
#include "ShooterCharacter.h"
#include "Components/ShooterCharacterCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Curves/CurveVector.h"
#include "ShooterCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(ItemLog, All, All)

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	PickUpWidget->SetupAttachment(GetRootComponent());

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());

	
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	PickUpWidget->SetVisibility(false);
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::BeginOverlapAgroSphere);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::EndOverlapAgroSphere);
	
	SetRare();
	SetItemProperties(ItemStates);
}

void AItem::OnConstruction(const FTransform& Transform)
{
	SetRarity();
	SetColorFresnel();
	SetDepthStencilValue();
}

void AItem::PlayPickupSound(AShooterCharacter* Char)
{
	const auto PickupComponent = Cast<UPickupComponent>(Char->GetComponentByClass(UPickupComponent::StaticClass()));
	if (PickupComponent->GetbCanPlayPickupSound()) { UGameplayStatics::PlaySound2D(this, GetPickupSound()); }
	PickupComponent->HoldPickupSound();
}

void AItem::StartTimerGlowMaterial()
{
	if (GetItemStates() != EItemStates::EIS_Pickup) return;
	GetWorldTimerManager().SetTimer(GlowMaterial.TimerGlowMaterial,this, &AItem::StartTimerGlowMaterial, GlowMaterial.TimeGlowMaterial);
}

void AItem::UpdateGlowMaterial()
{
	if(!MeshMaterialDynamic) return;
	float ElapsedTime;
	FVector CurveVector;
	switch (GetItemStates())
	{
	case EItemStates::EIS_Equipped:
		break;
	case EItemStates::EIS_Pickup:
		if (!GlowMaterial.MaterialCurve) break;
		ElapsedTime = GetWorldTimerManager().GetTimerElapsed(GlowMaterial.TimerGlowMaterial);
		CurveVector = GlowMaterial.MaterialCurve->GetVectorValue(ElapsedTime);
		break;
	case EItemStates::EIS_EquipInterping:
		if (!GlowMaterial.InterpItemCurve) break;
		ElapsedTime = GetWorldTimerManager().GetTimerElapsed(TimerInterpCurve);
		CurveVector = GlowMaterial.InterpItemCurve->GetVectorValue(ElapsedTime);
		break;
	case EItemStates::EIS_PickedUp:
		break;
	case EItemStates::EIS_Falling:
		break;
	case EItemStates::EIS_MAX:
		break;
	default:
		break;
	}
	MeshMaterialDynamic->SetScalarParameterValue(TEXT("FresnelMultiplay"), CurveVector.X * GlowMaterial.FresnelMultiplay);
	MeshMaterialDynamic->SetScalarParameterValue(TEXT("ExponentFresnel"), CurveVector.Y * GlowMaterial.ExponentFresnel);
	MeshMaterialDynamic->SetScalarParameterValue(TEXT("FresnelFraction"), CurveVector.Z * GlowMaterial.FresnelFraction);
}

void AItem::SetColorFresnel()
{
	if (MeshMaterial)
	{
		MeshMaterialDynamic = UMaterialInstanceDynamic::Create(MeshMaterial, this);
		MeshMaterialDynamic->SetVectorParameterValue(TEXT("Color Fresnel"), RarityRow.GlowColor);
		GetMeshComponent()->SetMaterial(MaterialIndex, MeshMaterialDynamic);
	}
}

void AItem::SetDepthStencilValue()
{
	if (GetMeshComponent())
	{
		GetMeshComponent()->SetCustomDepthStencilValue(RarityRow.CustomDepthStencil);
	}
}

void AItem::BeginOverlapAgroSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AShooterCharacter* Char = Cast<AShooterCharacter>(OtherActor);
		if (Char)
		{
			Char->IncrementAgroCountItem(1);
			ToggleCustomDepth(true);
		}
	}	
}

void AItem::EndOverlapAgroSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AShooterCharacter* Char = Cast<AShooterCharacter>(OtherActor);
		if (Char)
		{
			Char->IncrementAgroCountItem(-1);
			ToggleCustomDepth(false);
		}
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ItemInterp(DeltaTime);
	UpdateGlowMaterial();
}

void AItem::SetRare()
{
	for (int32 x = 0; x < 5; x++)
	{
		ActiveStars.Add(false);
	}

	switch (ItemRare)
	{
	case EItemRare::EIR_Damaged:
		ActiveStars[0] = true;
		break;
	case EItemRare::EIR_Common:
		ActiveStars[0] = true;
		ActiveStars[1] = true;
		break;
	case EItemRare::EIR_Uncommon:
		ActiveStars[0] = true;
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		break;
	case EItemRare::EIR_Rare:
		ActiveStars[0] = true;
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		break;
	case EItemRare::EIR_Legendary:
		ActiveStars[0] = true;
		ActiveStars[1] = true;
		ActiveStars[2] = true;
		ActiveStars[3] = true;
		ActiveStars[4] = true;
		break;
	case EItemRare::EIR_MAX:
		break;
	default:
		break;
	}
}

void AItem::SetItemStates(EItemStates State)
{
	ItemStates = State;
	SetItemProperties(State);
}

void AItem::StartPickupItem(AShooterCharacter* Char)
{
	if (!Char) return;
	Character = Char;
	PlayPickupSound(Char);
	IsBeTaken = true;
	ItemStartCurveLocation = GetActorLocation();

	SetItemStates(EItemStates::EIS_EquipInterping);
	//ToggleCustomDepth(true);
	GetWorldTimerManager().SetTimer(TimerInterpCurve, this, &AItem::FinishInterpItem, InterpTimeCurve);

	const auto CameraComponent = Cast<UCameraComponent>(Char->GetComponentByClass(UCameraComponent::StaticClass()));
	if (!CameraComponent) return;
	const float InitialCameraYaw = CameraComponent->GetComponentRotation().Yaw;
	const float InitialItemYaw = GetActorRotation().Yaw;
	InterpInitialYawOffset = InitialItemYaw - InitialCameraYaw;
}

void AItem::GetPickupItem()
{
}

void AItem::SetItemProperties(EItemStates State)
{
	switch (State)
	{
	case EItemStates::EIS_Equipped:
		PickUpWidget->SetVisibility(false);
		//set properties for Mesh Item
		GetMeshComponent()->SetSimulatePhysics(false);
		GetMeshComponent()->SetVisibility(true);
		GetMeshComponent()->SetEnableGravity(false);
		GetMeshComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Set Collision properties for Sphere item
		AgroSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//SetCollision properties for CollisionBox
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//effects
		ToggleCustomDepth(false);
		ToggleGlowMaterial(false);
		break;

	case EItemStates::EIS_Pickup:
		//set properties for Mesh Item
		GetMeshComponent()->SetSimulatePhysics(false);
		GetMeshComponent()->SetVisibility(true);
		GetMeshComponent()->SetEnableGravity(false);
		GetMeshComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Set Collision properties for Sphere item
		AgroSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		AgroSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//SetCollision properties for CollisionBox
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//effects
		ToggleCustomDepth(false);
		ToggleGlowMaterial(true);
		StartTimerGlowMaterial();
		break;

	case EItemStates::EIS_EquipInterping:
		PickUpWidget->SetVisibility(false);
		//set properties for Mesh Item
		GetMeshComponent()->SetSimulatePhysics(false);
		GetMeshComponent()->SetVisibility(true);
		GetMeshComponent()->SetEnableGravity(false);
		GetMeshComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Set Collision properties for Sphere item
		AgroSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//SetCollision properties for CollisionBox
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//effects
		ToggleCustomDepth(true);
		ToggleGlowMaterial(true);
		break;

	case EItemStates::EIS_PickedUp:
		PickUpWidget->SetVisibility(false);
		//set properties for Mesh Item
		GetMeshComponent()->SetSimulatePhysics(false);
		GetMeshComponent()->SetVisibility(false);
		GetMeshComponent()->SetEnableGravity(false);
		GetMeshComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Set Collision properties for Sphere item
		AgroSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//SetCollision properties for CollisionBox
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemStates::EIS_Falling:
		//set properties for Mesh Item
		GetMeshComponent()->SetSimulatePhysics(true);
		GetMeshComponent()->SetVisibility(true);
		GetMeshComponent()->SetEnableGravity(true);
		GetMeshComponent()->SetVisibility(true);
		GetMeshComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetMeshComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		GetMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//Set Collision properties for Sphere item
		AgroSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//SetCollision properties for CollisionBox
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//effects
		ToggleCustomDepth(true);
		ToggleGlowMaterial(true);
		break;

	case EItemStates::EIS_MAX:
		break;

	default:
		break;
	}
}

//call when TimerCurve end / stop interp movement to camera char
void AItem::FinishInterpItem()
{
	IsBeTaken = false;
	if (Character)
	{
		Character->GetPickupItem(this);
	}
	SetActorScale3D(FVector(1.f));
	//ToggleCustomDepth(false);
	//ToggleGlowMaterial(false);
	//ToggleCustomDepth(false);
}


void AItem::ItemInterp(float DeltaTime)
{
	if(!IsBeTaken || !Character || !ItemZCurve) return;

	//get start/target locations
	const FVector StartLocation = ItemStartCurveLocation;
	const FVector FinishLocation = Character->GetInterpLocation(this);

	//get z size between finish and start location
	const FVector StartToFinishLocation = FVector {0.f, 0.f, (FinishLocation - StartLocation).Z};
	const float DeltaZ = StartToFinishLocation.Size();

	//Get current value CurveZ when timer elapsed
	const float CurrentElapsedTime = GetWorldTimerManager().GetTimerElapsed(TimerInterpCurve);
	const float CurrentCurveZ = ItemZCurve->GetFloatValue(CurrentElapsedTime);

	//get current actor location
	const FVector CurrentItemLocation = { GetActorLocation() };
	//interp X Y item location
	const float InterpXItemLocation = FMath::FInterpTo(CurrentItemLocation.X, FinishLocation.X, DeltaTime, 30.f);
	const float InterpYItemLocation = FMath::FInterpTo(CurrentItemLocation.Y, FinishLocation.Y, DeltaTime, 30.f);
	
	//Set XYZ Item location every tick 
	const FVector TickLocation = FVector(InterpXItemLocation, InterpYItemLocation, CurrentCurveZ * DeltaZ);
	SetActorLocation(TickLocation, false, nullptr, ETeleportType::TeleportPhysics);

	//Set Item Rotation when we interp
	const auto CameraComponent = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));
	if (!CameraComponent) return;
	const FRotator CurrentCameraRotaion = CameraComponent->GetComponentRotation();
	const FRotator ItemRotation{ 0.f, CurrentCameraRotaion.Yaw + InterpInitialYawOffset, 0.f };
	SetActorRotation(ItemRotation, ETeleportType::TeleportPhysics);

	if (ItemScaleCurve)
	{
		const float ScaleCurveValue = ItemScaleCurve->GetFloatValue(CurrentElapsedTime);
		SetActorScale3D(FVector(ScaleCurveValue));
	}
}

void AItem::ToggleCustomDepth(bool bEnableCustomDepth)
{
	GetMeshComponent()->SetRenderCustomDepth(bEnableCustomDepth);
}

void AItem::ToggleGlowMaterial(bool bEnableGlowMaterial)
{
	if (MeshMaterialDynamic)
	{
		MeshMaterialDynamic->SetScalarParameterValue(TEXT("AlphaBlend"), !bEnableGlowMaterial);
	}
}

void AItem::SetRarity()
{
	//FString RarityTablePath(TEXT("DataTable'/Game/_Shooter/DataTables/DT_ItemRarity.DT_ItemRarity'"));
	//UDataTable* RarityTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *RarityTablePath));
	if (RarityTableData)
	{
		TMap<EItemRare, FName> RowName{ 
			{EItemRare::EIR_Damaged, TEXT("Damaged")},
			{EItemRare::EIR_Common, TEXT("Common")},
			{EItemRare::EIR_Uncommon, TEXT("Uncommon")},
			{EItemRare::EIR_Rare, TEXT("Rare")},
			{EItemRare::EIR_Legendary, TEXT("Legendary")}, };		
		if(!RowName.Contains(ItemRare)) return;
		RarityRow = *RarityTableData->FindRow<FItemRarityTable>(*RowName.Find(ItemRare), TEXT(""));

		//switch (ItemRare)
		//{
		//case EItemRare::EIR_Damaged:
		//	RarityRow = *RarityTableObject->FindRow<FItemRarityTable>(FName("Damaged"), TEXT(""));
		//	break;
		//case EItemRare::EIR_Common:
		//	RarityRow = *RarityTableObject->FindRow<FItemRarityTable>(FName("Common"), TEXT(""));
		//	break;
		//case EItemRare::EIR_Uncommon:
		//	RarityRow = *RarityTableObject->FindRow<FItemRarityTable>(FName("Uncommon"), TEXT(""));
		//	break;
		//case EItemRare::EIR_Rare:
		//	RarityRow = *RarityTableObject->FindRow<FItemRarityTable>(FName("Rare"), TEXT(""));
		//	break;
		//case EItemRare::EIR_Legendary:
		//	RarityRow = *RarityTableObject->FindRow<FItemRarityTable>(FName("Legendary"), TEXT(""));
		//	break;
		//}
	}
}

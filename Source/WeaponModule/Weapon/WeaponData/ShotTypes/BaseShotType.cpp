// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShotType.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/CharacterWeapon.h"
#include "Weapon/WeaponData/AmmoPatterns/BaseAmmoPattern.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


//Replications
bool UBaseShotType::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	if (!GetOuter()) return false;
	UNetDriver* NetDriver = Cast<AActor>(GetOuter())->GetNetDriver();
	if (!NetDriver) return false;
	NetDriver->ProcessRemoteFunction(Cast<AActor>(GetOuter()), Function, Parms, OutParms, Stack, this);
	return true;
}
int32 UBaseShotType::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	return (GetOuter() ? GetOuter()->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local);
}
//----------------------------------------------------------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------------------------------------------------------
FHitResult UBaseShotType::TraceLocationBeforeCharacter(float Distance, FVector_NetQuantize ViewLocation,
                                                       FRotator ViewRotation, bool bReturnPhysParams, float Spread,
                                                       int32 Seed) const
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(TArray<AActor*>{GetWeapon(), GetWeapon()->GetOwner()});
	CollisionParams.bReturnPhysicalMaterial = bReturnPhysParams;

	FHitResult HitResult;
	const FVector TraceStart = ViewLocation;

	FRandomStream Stream;
	if(Seed == -1)
		UKismetMathLibrary::SeedRandomStream(Stream);
	else
		UKismetMathLibrary::SetRandomStreamSeed(Stream, Seed);
	
	const FVector ShootDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegreesFromStream(
            ViewRotation.Vector(), Spread, Stream);	
	const FVector TraceEnd = TraceStart + ShootDirection * Distance;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
	
	return HitResult;
}

void UBaseShotType::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UBaseShotType, RecoilPower, COND_OwnerOnly)
}

FVector UBaseShotType::GetMuzzleWorldLocation() const
{
	const FVector MuzzleLocation = (Cast<USkeletalMeshComponent>(
		GetWeapon()->GetComponentByClass(USkeletalMeshComponent::StaticClass())))->GetSocketLocation(MuzzleSocketName);
	return MuzzleLocation;
}

void UBaseShotType::GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetWeapon()->GetOwnerCharacter()->GetController();
	if(!Controller) return;

	if(Cast<APlayerController>(Controller))
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	else
		Controller->GetActorEyesViewPoint(ViewLocation, ViewRotation);
}


void UBaseShotType::TryShoot()
{
	Shoot();
	Recoil();
}

void UBaseShotType::Recoil_Implementation() const
{
	GetWeapon()->GetOwnerCharacter()->AddControllerPitchInput(FMath::RandRange(-RecoilPower, 0.0f));
	GetWeapon()->GetOwnerCharacter()->AddControllerYawInput(FMath::RandRange(-RecoilPower/2, RecoilPower/2));
}

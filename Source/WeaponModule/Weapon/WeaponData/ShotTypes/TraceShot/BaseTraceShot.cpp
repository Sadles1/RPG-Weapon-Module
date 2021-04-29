// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTraceShot.h"
#include "Components/WeaponFXComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/CharacterWeapon.h"

void UBaseTraceShot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseTraceShot, TraceSpread)
	DOREPLIFETIME(UBaseTraceShot, TraceDistance)
}

void UBaseTraceShot::Shoot()
{
	const uint8 Seed = FMath::RandRange(0, 255);

	FVector ViewLocation;
	FRotator ViewRotation;
	GetViewPoint(ViewLocation, ViewRotation);
	
	const TArray<FHitResult> HitResults = Trace(Seed, ViewLocation, ViewRotation);
	ShootMulticast(Seed, ViewLocation, ViewRotation);
	
	MakeDamage(HitResults);
}


void UBaseTraceShot::ShootMulticast_Implementation(const uint8 Seed, FVector_NetQuantize ViewLocation, FRotator ViewRotation)
{
	if(GetWeapon()->HasAuthority()) return;
	const TArray<FHitResult> HitResults = Trace(Seed, ViewLocation, ViewRotation);
	GetWeapon()->GetWeaponFX()->ShotFX(HitResults);
}


TArray<FHitResult> UBaseTraceShot::Trace(int32 SeedToRandom, FVector_NetQuantize ViewLocation, FRotator ViewRotation)
{
	return {};
}

void UBaseTraceShot::MakeDamage(TArray<FHitResult> HitResults)
{
	if(HitResults.Num() > 0)
		for(auto HitResult : HitResults)
		{
			if(HitResult.GetActor())
				HitResult.GetActor()->TakeDamage(GetDamage(), FDamageEvent(),
				                                 GetWeapon()->GetOwnerCharacter()->GetController(), GetWeapon());
		}
}

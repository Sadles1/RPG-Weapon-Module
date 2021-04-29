// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFireMode.h"


#include "Net/UnrealNetwork.h"
#include "Weapon/CharacterWeapon.h"
#include "Weapon/WeaponData/AmmoPatterns/BaseAmmoPattern.h"
#include "Weapon/WeaponData/ShotTypes/BaseShotType.h"

void UBaseFireMode::Initialize(ACharacterWeapon* NewWeapon)
{
	Super::Initialize(NewWeapon);
	
	ShotType->Initialize(NewWeapon);
	AmmoPattern->Initialize(NewWeapon);
}

bool UBaseFireMode::CanStartFire() const
{
	return ShotType->CanShoot() && AmmoPattern->CanUseAmmo() && !TimeBetweenShots();
}

void UBaseFireMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UBaseFireMode, AmmoPattern, COND_OwnerOnly)
	DOREPLIFETIME(UBaseFireMode, ShotType)
}

void UBaseFireMode::TryStartFire()
{
	if(CanStartFire())
	{
		bIsFiring = true;

		
		bIsTimeBetweenShots = true;
		GetWorld()->GetTimerManager().SetTimer(BetweenShotsTimer, this, &UBaseFireMode::EndTimeBetweenShots, 60/GetFireRate(), false);
		
		StartFire();
	}
}

void UBaseFireMode::TryStopFire()
{
	if(bIsFiring)
	{
		bIsFiring = false;	
		StopFire();
	}
}

void UBaseFireMode::StopAll()
{
	TryStopFire();

	AmmoPattern->StopAll();
}

void UBaseFireMode::EndTimeBetweenShots()
{
	bIsTimeBetweenShots = false;
}

void UBaseFireMode::StartFire() {}

void UBaseFireMode::StopFire() {}

void UBaseFireMode::MakeShot() const
{
	if(ShotType->CanShoot() && AmmoPattern->CanUseAmmo())
	{
		ShotType->TryShoot();
		AmmoPattern->UseAmmo();
		
		GetWeapon()->PlayMontageOnWeapon(FireWeaponMontage, 60/GetFireRate());
        GetWeapon()->PlayMontageOnCharacter(FireMontage, 60/GetFireRate());
	}
}

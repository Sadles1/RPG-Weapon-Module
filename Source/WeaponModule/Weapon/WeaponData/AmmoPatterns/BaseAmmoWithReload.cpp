// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAmmoWithReload.h"

#include "Net/UnrealNetwork.h"
#include "Weapon/CharacterWeapon.h"

bool UBaseAmmoWithReload::CanUseAmmo()
{
	return GetCurrentAmmo() >= GetAmmoForOneShot() && (!bIsReloading || CanAbortReload());
}

bool UBaseAmmoWithReload::CanReload()
{
	return GetCurrentAmmo() < GetAmmoInClip() && !bIsReloading;
}

void UBaseAmmoWithReload::UseAmmo()
{
	if(CanAbortReload())
		StopAll();
	
	CurrentAmmo -= GetAmmoForOneShot();
	
	UE_LOG(LogTemp, Display, TEXT("%i"), GetCurrentAmmo());
	if(GetCurrentAmmo() < GetAmmoForOneShot())
	{
		GetWeapon()->StopFire();
	}
}

void UBaseAmmoWithReload::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UBaseAmmoWithReload, CurrentAmmo, COND_OwnerOnly)
}

void UBaseAmmoWithReload::StopAll()
{
	Super::StopAll();
	
	if(bIsReloading)
	{
		bIsReloading = false;
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
	}
}

void UBaseAmmoWithReload::ReloadAllAmmo()
{
	const int32 AmmoToReload = GetAmmoInClip() - GetCurrentAmmo();
	if(AmmoToReload <= GetTotalAmmo())
	{
		CurrentAmmo = GetAmmoInClip();
		TotalAmmo -= AmmoToReload;
	}
	else
	{
		CurrentAmmo += GetTotalAmmo();
		TotalAmmo = 0;
	}
}

void UBaseAmmoWithReload::TryReload()
{
	if(CanReload())
	{
		bIsReloading = true;

		GetWeapon()->StopFire();
		GetWeapon()->PlayMontageOnCharacter(ReloadMontage, GetReloadTime());
		GetWeapon()->PlayMontageOnWeapon(WeaponReloadMontage, GetReloadTime());

		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &UBaseAmmoWithReload::EndReload, GetReloadTime() * 0.90, false);
		
	}
}

void UBaseAmmoWithReload::EndReload()
{
	if(bIsReloading)
	{		
		if(bEachAmmoReload)
		{		
			CurrentAmmo++;
			if(GetCurrentAmmo() >= GetAmmoInClip())
				bIsReloading = false;
			else
			{
				GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &UBaseAmmoWithReload::EndReload, GetReloadTime() * 0.90, false);
				
				GetWeapon()->PlayMontageOnWeapon(WeaponReloadMontage, GetReloadTime());
				GetWeapon()->PlayMontageOnCharacter(ReloadMontage, GetReloadTime());
			}
		}
		else
		{
			bIsReloading = false;
			ReloadAllAmmo();
		}
	}
}

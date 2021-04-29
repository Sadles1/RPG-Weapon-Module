// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAmmoPattern.h"
#include "BaseAmmoWithReload.generated.h"


class UAnimMontage;

UCLASS()
class WEAPONMODULE_API UBaseAmmoWithReload : public UBaseAmmoPattern
{
	GENERATED_BODY()

public:
	virtual bool CanUseAmmo() override;
	virtual bool CanReload();
	
	bool CanAbortReload() const {return bEachAmmoReload && bIsReloading;}
	
	virtual void UseAmmo() override;
	

	void TryReload();

	//-------------------------------------------------------------------------
	//Getters
	int32 GetCurrentAmmo() const {return CurrentAmmo;}
	int32 GetAmmoInClip() const {return AmmoInClip;}
	int32 GetTotalAmmo() const {return TotalAmmo;}
	int32 GetAmmoForOneShot() const	{return AmmoForOneShot;}
	float GetReloadTime() const {return ReloadTime;}

private:

	bool bIsReloading = false;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	virtual void StopAll() override;
	
	void ReloadAllAmmo();
	void EndReload();

	
	UPROPERTY(EditAnywhere, Replicated, Category = "Ammo")
	int32 CurrentAmmo = 30;
	
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 AmmoInClip = 30;
	int32 TotalAmmo = 400;
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 AmmoForOneShot = 1;

	UPROPERTY(EditAnywhere, Category = "Reload")
	bool bEachAmmoReload = false;
	UPROPERTY(EditAnywhere, Category = "Reload")
	UAnimMontage* ReloadMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Reload")
	UAnimSequence* WeaponReloadMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Reload")
	float ReloadTime = 3.f;

	FTimerHandle ReloadTimer;
	
};

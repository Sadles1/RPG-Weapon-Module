// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponObject.h"
#include "WeaponData/ShotTypes/BaseShotType.h"

#include "BaseFireMode.generated.h"


class UBaseAmmoPattern;
class UBaseShotType;
class UAnimMontage;

UCLASS(Abstract)
class WEAPONMODULE_API UBaseFireMode : public UWeaponObject
{
	GENERATED_BODY()

	friend class ACharacterWeapon;
	
public:
	virtual void Initialize(ACharacterWeapon* NewWeapon) override;
	
	
	
	bool TimeBetweenShots() const {return bIsTimeBetweenShots;}

	
	UBaseAmmoPattern* GetAmmoPattern() const {return AmmoPattern;}

	FName GetShotFX() const {return ShotType->GetShotFX();}
	
protected:
	
	void MakeShot() const;

	
	float GetFireRate() const {return FireRate;}

	

private:
	bool CanStartFire() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void TryStartFire();
	void TryStopFire();
	void StopAll();

	
	bool bIsTimeBetweenShots = false;
	FTimerHandle BetweenShotsTimer;

	
	void EndTimeBetweenShots();

	virtual void StartFire();
	virtual void StopFire();
	
	UPROPERTY(EditAnywhere, Category = "Fire")
	float FireRate = 300.f;

	
	bool bIsFiring = false;

	UPROPERTY(EditAnywhere, Category = "Animation's")
	UAnimMontage* FireMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = "Animation's")
	UAnimSequence* FireWeaponMontage = nullptr;
	
	UPROPERTY(EditAnywhere, Replicated, Instanced, Category = "Shot")
	UBaseShotType* ShotType = nullptr;
	UPROPERTY(EditAnywhere, Replicated, Instanced, Category = "Ammo's")
	UBaseAmmoPattern* AmmoPattern = nullptr;
};

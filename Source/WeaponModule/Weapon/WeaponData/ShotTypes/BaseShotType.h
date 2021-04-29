// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponObject.h"
#include "BaseShotType.generated.h"


class USoundCue;
class UBaseAmmoPattern;
class UPhysicalMaterial;
class UNiagaraSystem;


UCLASS(Abstract)
class WEAPONMODULE_API UBaseShotType : public UWeaponObject
{
	GENERATED_BODY()

public:
	void TryShoot();

	bool CanShoot() const { return true; }

	FName GetShotFX() const { return ShotFXDataRowName; }


	virtual bool CallRemoteFunction(UFunction* Function, void* Parms,
	                                struct FOutParmRec* OutParms, FFrame* Stack) override;

	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;


protected:

	float GetDamage() const { return Damage; }

	UFUNCTION(Client, Reliable)
	void Recoil() const;

	FHitResult TraceLocationBeforeCharacter(float Distance = 3000.f,
	                                        FVector_NetQuantize ViewLocation = FVector::ZeroVector,
	                                        FRotator ViewRotation = FRotator::ZeroRotator,
	                                        bool bReturnPhysParams = false,
	                                        float Spread = 0.f,
	                                        int32 Seed = -1) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FVector GetMuzzleWorldLocation() const;

	void GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

private:
	UPROPERTY(EditAnywhere, Category = "Shot")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, Replicated, Category = "Shot")
	float RecoilPower = 0.15f;

	UPROPERTY(EditAnywhere, Category = "Shot")
	FName MuzzleSocketName = "MuzzleFlash";
	UPROPERTY(EditAnywhere, Category = "Shot", meta = (DisplayName = "FX Data Row Name"))
	FName ShotFXDataRowName = "Rifle";

	virtual void Shoot()
	{
	};
};

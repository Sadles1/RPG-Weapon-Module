// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponData/ShotTypes/BaseShotType.h"
#include "BaseTraceShot.generated.h"


UCLASS(Abstract)
class WEAPONMODULE_API UBaseTraceShot : public UBaseShotType
{
	GENERATED_BODY()
protected:
	float GetTraceDistance() const { return TraceDistance; }
	float GetTraceSpread() const { return TraceSpread; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	virtual void Shoot() override;

	UFUNCTION(NetMulticast, Unreliable)
	void ShootMulticast(uint8 Seed, FVector_NetQuantize ViewLocation, FRotator ViewRotation);

	virtual TArray<FHitResult> Trace(int32 SeedToRandom, FVector_NetQuantize ViewLocation, FRotator ViewRotation);
	virtual void MakeDamage(TArray<FHitResult> HitResults);


	UPROPERTY(EditAnywhere, Replicated, Category = "Trace")
	float TraceDistance = 5000.f;

	UPROPERTY(EditAnywhere, Replicated, Category = "Trace")
	float TraceSpread = 1.5f;
};

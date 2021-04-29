// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LineTraceShot.h"
#include "ExplosionTraceShot.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONMODULE_API UExplosionTraceShot : public ULineTraceShot
{
	GENERATED_BODY()

	

private:	
	virtual void MakeDamage(TArray<FHitResult> HitResults) override;

	float GetExplosionRadius() const {return ExplosionRadius;}

	UPROPERTY(EditAnywhere, Category = "Explosion")
	float ExplosionRadius = 300.f;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	bool bDoFullDamage = false;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	bool bDamageSelf = false;

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BaseTraceShot.h"
#include "ShotgunTraceShot.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONMODULE_API UShotgunTraceShot : public UBaseTraceShot
{
	GENERATED_BODY()
	

protected:
	virtual TArray<FHitResult> Trace(int32 Seed, FVector_NetQuantize ViewLocation, FRotator ViewRotation) override;


private:
	UPROPERTY(EditAnywhere, Category = "Trace")
	int32 NumPellets = 10;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTraceShot.h"
#include "LineTraceShot.generated.h"


UCLASS()
class WEAPONMODULE_API ULineTraceShot : public UBaseTraceShot
{
	GENERATED_BODY()

protected:
	virtual TArray<FHitResult> Trace(int32 Seed, FVector_NetQuantize ViewLocation, FRotator ViewRotation) override;
};

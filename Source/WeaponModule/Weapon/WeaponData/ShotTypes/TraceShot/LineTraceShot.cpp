// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTraceShot.h"

TArray<FHitResult> ULineTraceShot::Trace(const int32 Seed, FVector_NetQuantize ViewLocation, FRotator ViewRotation)
{
	return {TraceLocationBeforeCharacter(GetTraceDistance(), ViewLocation, ViewRotation, true, GetTraceSpread(), Seed)};
}
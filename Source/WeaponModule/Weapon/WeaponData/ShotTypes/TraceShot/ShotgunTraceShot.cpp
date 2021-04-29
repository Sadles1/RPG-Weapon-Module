// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunTraceShot.h"


TArray<FHitResult> UShotgunTraceShot::Trace(int32 Seed, FVector_NetQuantize ViewLocation, FRotator ViewRotation)
{
	TArray<FHitResult> HitResults;
	for(int i=0; i <= NumPellets; i++)
	{
		Seed++;
		HitResults.Add(TraceLocationBeforeCharacter(GetTraceDistance(), ViewLocation, ViewRotation, true,  GetTraceSpread(), Seed));
	}
	return HitResults;
}

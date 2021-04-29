// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomaticFireMode.h"

void UAutomaticFireMode::StartFire()
{
	GetWorld()->GetTimerManager().SetTimer(AutomaticFireTimer, this, &UAutomaticFireMode::MakeShot, 60.f/GetFireRate(), true, 0.f);
}

void UAutomaticFireMode::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimer);
}

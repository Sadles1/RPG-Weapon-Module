// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponObject.h"
#include "CharacterWeapon.h"
#include "Net/UnrealNetwork.h"

void UWeaponObject::Initialize(ACharacterWeapon* NewWeapon)
{
	Rename(nullptr, NewWeapon);
	WeaponOwner = NewWeapon;
}

void UWeaponObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponObject, WeaponOwner);
}

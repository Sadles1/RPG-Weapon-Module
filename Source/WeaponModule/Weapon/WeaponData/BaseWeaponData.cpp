// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponData.h"

#include "CharacterWeapon.h"
#include "Components/WeaponAnimComponent.h"
#include "Components/WeaponFXComponent.h"
#include "Engine/ObjectLibrary.h"
#include "FireModes/BaseFireMode.h"
#include "Net/UnrealNetwork.h"

void UBaseWeaponData::Initialize(ACharacterWeapon* NewWeapon)
{ 	
	Super::Initialize(NewWeapon);
	
	if(AllFireModes.Num() > 0)
	{
		for(auto FireMode : AllFireModes)
			FireMode->Initialize(NewWeapon);
		if(!CurrentFireMode)
			CurrentFireMode = AllFireModes[0];
	}
}

void UBaseWeaponData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UBaseWeaponData, CurrentFireMode, COND_OwnerOnly);
}

void UBaseWeaponData::SelectedWeapon() const
{
	GetWeapon()->GetWeaponFX()->SetShotFX(CurrentFireMode->GetShotFX());
	GetWeapon()->GetWeaponAnim()->SetAnim(WeaponAnimationDataName);
}

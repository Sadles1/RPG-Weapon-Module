// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimComponent.h"
#include "CharacterWeapon.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UWeaponAnimComponent::UWeaponAnimComponent()
{
	SetIsReplicatedByDefault(true);
}

void UWeaponAnimComponent::SetAnim(const FName NewDataTableRowName)
{
	DataTableRowName = NewDataTableRowName;

	if(IsNetMode(NM_ListenServer) || IsNetMode(NM_Standalone))
		OnRep_RowName();
}


// Called when the game starts
void UWeaponAnimComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UWeaponAnimComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponAnimComponent, DataTableRowName)
}

void UWeaponAnimComponent::OnRep_RowName()
{
	if(AnimationsDataTable)
	{
		static const FString ContextString(TEXT("Shot fx data context"));

		FCharacterWeaponAnimations* Anim = AnimationsDataTable->FindRow<FCharacterWeaponAnimations>(DataTableRowName, ContextString, true);

		if(Anim)
		{
			WeaponAnimations = *Anim;
		}
	}
}

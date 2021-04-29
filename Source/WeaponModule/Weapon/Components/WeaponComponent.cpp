// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "Engine/DemoNetDriver.h"
#include "Weapon/CharacterWeapon.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner()->HasAuthority())
	{
		for (auto Weapon : StartWeapons)
		{
			EquipWeapon(Weapon);
		}
	}
	
	StartWeapons.Empty();
}


void UWeaponComponent::StartFire_Implementation() const
{
	if (CurrentWeapon)
		CurrentWeapon->StartFire();
}

void UWeaponComponent::StopFire_Implementation() const
{
	if (CurrentWeapon)
		CurrentWeapon->StopFire();
}


void UWeaponComponent::Reload_Implementation() const
{
	if (CurrentWeapon)
		CurrentWeapon->Reload();
}


void UWeaponComponent::EquipWeapon(UBaseWeaponData* WeaponData, const int32 WeaponSlotIndex)
{
	if (!EquipWeaponClass || !GetOwner()->HasAuthority()) return;

	ACharacterWeapon* NewWeapon = GetWorld()->SpawnActorDeferred<ACharacterWeapon>(
        EquipWeaponClass, FTransform::Identity, GetOwner());


	if (!NewWeapon) return;
	NewWeapon->FinishSpawning(FTransform::Identity);
	
	NewWeapon->SetWeaponData(WeaponData);

	if(WeaponSlotIndex == -1)
	{
		for (FWeaponSlot& WeaponSlot : WeaponSlots)
			if (CanEquipWeaponToSlot(WeaponData, WeaponSlot) && !WeaponSlot.WeaponInSlot)
			{
				WeaponSlot.WeaponInSlot = NewWeapon;
				NewWeapon->AttachToComponent(
					Cast<USceneComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass())),
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSlot.HolsterSocketName);
				return;
			}
	}
	else
	{
		FWeaponSlot& WeaponSlot = WeaponSlots[WeaponSlotIndex];
		if (CanEquipWeaponToSlot(WeaponData, WeaponSlot) && !WeaponSlot.WeaponInSlot)
		{
			WeaponSlot.WeaponInSlot = NewWeapon;
		
		}
	}
}


void UWeaponComponent::SelectWeapon_Implementation(int32 SlotIndex)
{
	SlotIndex--;
	if (SlotIndex >= WeaponSlots.Num() || SlotIndex < 0 || bSwitchingWeapons) return;

	bSwitchingWeapons = true;

	if(CurrentWeapon)
	{
		CurrentWeapon->StopAll();
	}
	
	if (CurrentSlotIndex == SlotIndex)
		StartHolsterWeapon();
	else
	{
		StartTakeWeapon(SlotIndex);
	}
}

void UWeaponComponent::StartTakeWeapon(const int8 SlotIndex)
{
	if (SlotIndex >= WeaponSlots.Num() || SlotIndex < 0) return;
	const FWeaponSlot& WeaponSlot = WeaponSlots[SlotIndex];

	if(!WeaponSlot.WeaponInSlot || !GetOwner()->HasAuthority()) return;

	TakenSlotIndex = SlotIndex;

	if(CurrentWeapon)
	{
		StartHolsterWeapon();
		return;
	}
	
	WeaponSlot.WeaponInSlot->PlayMontageOnCharacter(WeaponSlot.TakeMontage, WeaponSlot.GetTakeTime());
	
	WeaponSlot.WeaponInSlot->SelectWeapon();
	
	GetWorld()->GetTimerManager().SetTimer(TakeWeaponTimer, this, &UWeaponComponent::EndTakeWeapon, WeaponSlot.GetTakeTime() * 0.80f, false);
}

void UWeaponComponent::EndTakeWeapon()
{
	if (TakenSlotIndex >= WeaponSlots.Num() || TakenSlotIndex < 0) return;

	AttachWeaponToHand();
	
	CurrentSlotIndex = TakenSlotIndex;
	TakenSlotIndex = -1;
	
	CurrentWeapon = WeaponSlots[CurrentSlotIndex].WeaponInSlot;
	
	bSwitchingWeapons = false;
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, CurrentWeapon)
}


void UWeaponComponent::StartHolsterWeapon()
{
	if (CurrentSlotIndex >= WeaponSlots.Num() || CurrentSlotIndex < 0) return;
	const FWeaponSlot& WeaponSlot = WeaponSlots[CurrentSlotIndex];

	if(!WeaponSlot.WeaponInSlot || !GetOwner()->HasAuthority()) return;

	HolstenSlotIndex = CurrentSlotIndex;

	CurrentWeapon = nullptr;
	CurrentSlotIndex = -1;

	WeaponSlot.WeaponInSlot->PlayMontageOnCharacter(WeaponSlot.HolsterMontage, WeaponSlot.GetHolsterTime());
	
	GetWorld()->GetTimerManager().SetTimer(TakeWeaponTimer, this, &UWeaponComponent::EndHolsterWeapon, WeaponSlot.GetHolsterTime() * 0.80f, false);
}
void UWeaponComponent::EndHolsterWeapon()
{
	if (HolstenSlotIndex >= WeaponSlots.Num() || HolstenSlotIndex < 0) return;

	AttachWeaponToHolster();
	
	HolstenSlotIndex = -1;

	GetOwner()->ForceNetUpdate();

	if(TakenSlotIndex != -1)
		StartTakeWeapon(TakenSlotIndex);
	else
		bSwitchingWeapons = false;
}


bool UWeaponComponent::CanEquipWeaponToSlot(UBaseWeaponData* WeaponData, const FWeaponSlot& WeaponSlot)
{
	if (WeaponData->GetWeaponType().MatchesAnyExact(WeaponSlot.AvailableWeaponsTypeForSlot))
	{
		return true;
	}

	return false;
}


void UWeaponComponent::AttachWeaponToHand_Implementation()
{
	if(TakenSlotIndex == -1) return;
	
	const FWeaponSlot& WeaponSlot = WeaponSlots[TakenSlotIndex];

	WeaponSlot.WeaponInSlot->AttachToComponent(
                    Cast<USceneComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass())),
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSlot.WeaponInSlot->GetHandSocket());
}

void UWeaponComponent::AttachWeaponToHolster_Implementation()
{
	if(HolstenSlotIndex == -1) return;
	
	const FWeaponSlot& WeaponSlot = WeaponSlots[HolstenSlotIndex];

	WeaponSlot.WeaponInSlot->AttachToComponent(
                    Cast<USceneComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass())),
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSlot.HolsterSocketName);
}

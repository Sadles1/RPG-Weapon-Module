// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWeapon.h"
#include "Engine/DemoNetDriver.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Character.h"
#include "WeaponData/BaseWeaponData.h"
#include "WeaponData/AmmoPatterns/BaseAmmoWithReload.h"
#include "WeaponData/FireModes/BaseFireMode.h"
#include "Components/WeaponFXComponent.h"
#include "Components/WeaponAnimComponent.h"
#include "Kismet/GameplayStatics.h"


ACharacterWeapon::ACharacterWeapon()
{
	bReplicates = true;

	NetCullDistanceSquared = 99999;
	NetUpdateFrequency = 30.f;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	if(SkeletalMeshComponent)
	{
		SetRootComponent(SkeletalMeshComponent);
	}

	WeaponFXComponent = CreateDefaultSubobject<UWeaponFXComponent>(TEXT("WeaponFX"));
	WeaponAnimComponent = CreateDefaultSubobject<UWeaponAnimComponent>(TEXT("WeaponAnim"));
}

void ACharacterWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterWeapon, WeaponData)
}

bool ACharacterWeapon::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (WeaponData) WroteSomething |= Channel->ReplicateSubobject(WeaponData , *Bunch, *RepFlags);
	if (WeaponData->CurrentFireMode) WroteSomething |= Channel->ReplicateSubobject(WeaponData->CurrentFireMode , *Bunch, *RepFlags);
	if (WeaponData->CurrentFireMode->ShotType) WroteSomething |= Channel->ReplicateSubobject(WeaponData->CurrentFireMode->ShotType , *Bunch, *RepFlags);
	if (WeaponData->CurrentFireMode->AmmoPattern) WroteSomething |= Channel->ReplicateSubobject(WeaponData->CurrentFireMode->AmmoPattern , *Bunch, *RepFlags);

	return WroteSomething;
}

void ACharacterWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterWeapon::PlayMontageOnCharacter_Implementation(UAnimMontage* AnimMontage, const float TimeToPlay) const
{
	if(AnimMontage && (!HasAuthority() || !IsNetMode(NM_DedicatedServer)))
	{
		ACharacter* Character = Cast<ACharacter>(GetOwnerCharacter());
		const float PlayRate = TimeToPlay > 0.f ? CalculateMontageRate(AnimMontage, TimeToPlay) : 1.f;
		
		Character->PlayAnimMontage(AnimMontage, PlayRate);
	}
}


void ACharacterWeapon::StopMontageOnCharacter_Implementation() const
{
	if(!HasAuthority() || !IsNetMode(NM_DedicatedServer))
	{
		ACharacter* Character = Cast<ACharacter>(GetOwnerCharacter());
		Character->StopAnimMontage();
	}
}

void ACharacterWeapon::OnConstruction(const FTransform& Transform)
{
	if(HasAuthority())
		if(WeaponData)
			if(WeaponData->WeaponMesh)
				SkeletalMeshComponent->SetSkeletalMesh(WeaponData->WeaponMesh);
}

float ACharacterWeapon::CalculateMontageRate(UAnimSequenceBase* Animation, const float TimeToPlay)
{
	if(!Animation) return 0.f;
	return Animation->GetPlayLength()/TimeToPlay;
}

void ACharacterWeapon::PlayMontageOnWeapon_Implementation(UAnimSequence* Animation, const float TimeToPlay) const
{
	
	if(Animation && (!HasAuthority() || !IsNetMode(NM_DedicatedServer)))
	{
		const float PlayRate = TimeToPlay > 0.f ? CalculateMontageRate(Animation, TimeToPlay) : 1.f;

		SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		SkeletalMeshComponent->SetAnimation(Animation);
		SkeletalMeshComponent->SetPlayRate(PlayRate);
		SkeletalMeshComponent->Play(false);
	}
}

void ACharacterWeapon::StartFire() const { WeaponData->CurrentFireMode->TryStartFire(); }
void ACharacterWeapon::StopFire() const { WeaponData->CurrentFireMode->TryStopFire(); }

void ACharacterWeapon::StopAll() const { WeaponData->CurrentFireMode->StopAll();}


void ACharacterWeapon::Reload() const
{
	UBaseAmmoWithReload* AmmoWithReload = Cast<UBaseAmmoWithReload>(WeaponData->CurrentFireMode->GetAmmoPattern());
	if(AmmoWithReload)
		AmmoWithReload->TryReload();
}

void ACharacterWeapon::SelectWeapon() const
{
	WeaponData->SelectedWeapon();
}

void ACharacterWeapon::SetWeaponData(UBaseWeaponData* NewWeaponData)
{
	NewWeaponData->Initialize(this);

	WeaponData = NewWeaponData;

	if(WeaponData->WeaponMesh)
		SkeletalMeshComponent->SetSkeletalMesh(WeaponData->WeaponMesh);
}

void ACharacterWeapon::OnRep_WeaponData() const
{
	if(WeaponData->WeaponMesh)
		SkeletalMeshComponent->SetSkeletalMesh(WeaponData->WeaponMesh);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Components/WeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/DemoNetDriver.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UWeaponFXComponent::UWeaponFXComponent()
{
	SetIsReplicatedByDefault(true);
}

USkeletalMeshComponent* UWeaponFXComponent::GetWeaponMesh() const
{
	return Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
}

void UWeaponFXComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponFXComponent, DataTableRowName)
}

void UWeaponFXComponent::OnRep_RowName()
{
	if (ShotFXDataTable)
	{
		static const FString ContextString(TEXT("Shot fx data context"));

		FShotFXData* ShotFX = ShotFXDataTable->FindRow<FShotFXData>(DataTableRowName, ContextString, true);

		if (ShotFX)
			ShotFXData = *ShotFX;
	}
}

void UWeaponFXComponent::SetShotFX(const FName NewDataTableRowName)
{
	DataTableRowName = NewDataTableRowName;

	if (IsNetMode(NM_ListenServer) || IsNetMode(NM_Standalone))
		OnRep_RowName();
}

void UWeaponFXComponent::ShotFX_Implementation(const TArray<FHitResult>& HitResults) const
{
	if (!GetOwner()->HasAuthority() || !IsNetMode(NM_DedicatedServer))
	{
		for (auto HitResult : HitResults)
		{
			const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;

			UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(), ShotFXData.TraceFX, GetWeaponMesh()->GetSocketLocation(MuzzleSocketName));

			if (TraceFXComponent)
				TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, EndPoint);

			const FImpactData ImpactData = ShotFXData.GetImpactData(HitResult);

			//Impact
			if (!HitResult.bBlockingHit && !ShotFXData.bSpawnImpactWithoutHit) return;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.ImpactEffect, EndPoint,
			                                               HitResult.ImpactNormal.Rotation());

			if (!HitResult.bBlockingHit) return;
			auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
				GetWorld(), ImpactData.DecalData.DecalMaterial, ImpactData.DecalData.Size, HitResult.ImpactPoint,
				HitResult.ImpactNormal.Rotation());

			if (DecalComponent)
			{
				DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
			}
		}
	}
}


//----------------------------------------------------------------------------------------------------------------------


const FImpactData& FShotFXData::GetImpactData(const FHitResult& Hit) const
{
	if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMat))
		{
			return ImpactDataMap[PhysMat];
		}
	}

	return DefaultImpactData;
}

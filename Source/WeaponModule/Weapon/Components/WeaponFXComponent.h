// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"

#include "WeaponFXComponent.generated.h"

class UNiagaraSystem;
struct FImpactData;



USTRUCT()
struct FImpactDecalData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Decal")
	UMaterialInstance* DecalMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector Size = FVector(10.f);

	UPROPERTY(EditAnywhere, Category = "Decal")
	float LifeTime = 5.f;

	UPROPERTY(EditAnywhere, Category = "Decal")
	float FadeOutTime = 0.7f;
	
};
USTRUCT()
struct FImpactData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* ImpactEffect = nullptr;


	UPROPERTY(EditAnywhere, Category = "Decal")
	FImpactDecalData DecalData;
};
USTRUCT(BlueprintType)
struct FShotFXData : public FTableRowBase
{
	GENERATED_BODY()

	const FImpactData& GetImpactData(const FHitResult& Hit) const;

	UPROPERTY(EditAnywhere, Category = "Trace")
	UNiagaraSystem* TraceFX = nullptr;

	UPROPERTY(EditAnywhere, Category = "Impact")
	bool bSpawnImpactWithoutHit = false;

private:
	
	UPROPERTY(EditAnywhere, Category = "Impact")
	FImpactData DefaultImpactData;

	UPROPERTY(EditAnywhere, NotReplicated, Category = "Impact")
	TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;

	
};

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONMODULE_API UWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWeaponFXComponent();

	UFUNCTION(NetMulticast, Client, Unreliable)
	void ShotFX(const TArray<FHitResult>& HitResults) const;
	
	void SetShotFX(const FName NewDataTableRowName);
	
protected:	
	USkeletalMeshComponent* GetWeaponMesh() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY()
	FString TraceTargetName = "TraceTarget";

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* ShotFXDataTable;
	
	UPROPERTY(VisibleAnywhere, Category = "Attach")
	FName MuzzleSocketName = "MuzzleFlash";

	UPROPERTY(ReplicatedUsing = OnRep_RowName)
	FName DataTableRowName = "None";

	UFUNCTION()
	void OnRep_RowName();

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	FShotFXData ShotFXData;
};

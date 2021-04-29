// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "WeaponAnimComponent.generated.h"

class UAimOffsetBlendSpace;
class UBlendSpace;

USTRUCT(BlueprintType)
struct FCharacterWeaponAnimations : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UBlendSpace* RelaxedBS = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAimOffsetBlendSpace* RelaxedAO = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UBlendSpace* AimBS = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAimOffsetBlendSpace* AimAO = nullptr;
};



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEAPONMODULE_API UWeaponAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponAnimComponent();

	void SetAnim(const FName NewDataTableRowName);

	FCharacterWeaponAnimations GetAnimations() const {return WeaponAnimations;}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* AnimationsDataTable;

	UPROPERTY(ReplicatedUsing = OnRep_RowName)
	FName DataTableRowName;

	UPROPERTY(VisibleAnywhere)
	FCharacterWeaponAnimations WeaponAnimations;

	UFUNCTION()
	void OnRep_RowName();
};

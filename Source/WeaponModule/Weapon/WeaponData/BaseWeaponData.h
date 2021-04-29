// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Weapon/WeaponObject.h"
#include "BaseWeaponData.generated.h"

class UBaseFireMode;
class USkeletalMesh;


UCLASS(Abstract, Blueprintable)
class WEAPONMODULE_API UBaseWeaponData : public UWeaponObject
{
	GENERATED_BODY()

	friend class ACharacterWeapon;

public:
	virtual void Initialize(ACharacterWeapon* NewWeapon) override;

	//------------------------------------------------------------------------------------------------------------------
	//Getters
	USkeletalMesh* GetWeaponMesh() const {return WeaponMesh;}
	FGameplayTag GetWeaponType() const {return WeaponType;}
	
protected:
	

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	USkeletalMesh* WeaponMesh = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FGameplayTag WeaponType;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName HandSocketName = "HandMainSocket";

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName WeaponAnimationDataName = "Rifle";

	void SelectedWeapon() const;
		
	UPROPERTY(Replicated, VisibleInstanceOnly, Instanced, Category = "FireMode's")
	UBaseFireMode* CurrentFireMode = nullptr;
	
	UPROPERTY(EditAnywhere, Instanced, Category = "FireMode's")
	TArray<UBaseFireMode*> AllFireModes = {};
};
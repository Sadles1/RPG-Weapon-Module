// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WeaponObject.generated.h"


class ACharacterWeapon;
UCLASS(Abstract, EditInlineNew)
class WEAPONMODULE_API UWeaponObject : public UObject
{
	GENERATED_BODY()
public:
	virtual void Initialize(ACharacterWeapon* NewWeapon);

	virtual bool IsSupportedForNetworking() const override {return true;}

	ACharacterWeapon* GetWeapon() const {return WeaponOwner;}

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	
	UPROPERTY(Replicated)
	ACharacterWeapon* WeaponOwner;
	
};

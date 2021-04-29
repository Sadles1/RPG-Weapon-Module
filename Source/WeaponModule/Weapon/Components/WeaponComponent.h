// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "Weapon/CharacterWeapon.h"
#include "WeaponComponent.generated.h"

class UBaseWeaponData;


USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SlotName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HolsterSocketName = "HolsterMainSocket";	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer AvailableWeaponsTypeForSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* TakeMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* HolsterMontage = nullptr;
	

	UPROPERTY(VisibleDefaultsOnly)
	ACharacterWeapon* WeaponInSlot = nullptr;

	float GetTakeTime() const {return TakeTime;}
	float GetHolsterTime() const {return HolsterTime;}

private:
	UPROPERTY(EditAnywhere, Category = "Time")
	float TakeTime = 0.8f;
	UPROPERTY(EditAnywhere, Category = "Time")
	float HolsterTime = 0.8f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONMODULE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void StartFire() const;	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void StopFire() const;

	

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Reload() const;
	


	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SelectWeapon(int32 SlotIndex);

	
	

protected:
	virtual void BeginPlay() override;
	
private:

	void StartTakeWeapon(const int8 SlotIndex);
	void EndTakeWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartHolsterWeapon();
	void EndHolsterWeapon();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AttachWeaponToHand();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AttachWeaponToHolster();

	bool CanEquipWeaponToSlot(UBaseWeaponData* WeaponData, const FWeaponSlot& WeaponSlot);
	void EquipWeapon(UBaseWeaponData* WeaponData, const int32 WeaponSlotIndex = -1);

	FTimerHandle TakeWeaponTimer;

	UPROPERTY(VisibleInstanceOnly, Replicated, BlueprintReadOnly, Category = "Weapon's", meta = (AllowPrivateAccess = "true"))
	ACharacterWeapon* CurrentWeapon = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon's")
	TSubclassOf<ACharacterWeapon> EquipWeaponClass = ACharacterWeapon::StaticClass();

	UPROPERTY(EditAnywhere, Category = "Slots")
	TArray<FWeaponSlot> WeaponSlots;

	UPROPERTY(EditAnywhere, Instanced, Category = "Start")
	TArray<UBaseWeaponData*> StartWeapons;
	
	bool bSwitchingWeapons = false;
	
	int8 CurrentSlotIndex = -1;
	int8 TakenSlotIndex = -1;
	int8 HolstenSlotIndex = -1;
};
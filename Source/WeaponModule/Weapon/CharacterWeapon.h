// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/WeaponAnimComponent.h"
#include "GameFramework/Actor.h"
#include "WeaponData/BaseWeaponData.h"
#include "CharacterWeapon.generated.h"


class USkeletalMeshComponent;
class UBaseWeaponData;
class UWeaponFXComponent;
class UWeaponAnimComponent;

UCLASS()
class WEAPONMODULE_API ACharacterWeapon : public AActor
{
	GENERATED_BODY()

public:
	ACharacterWeapon();
	
	APawn* GetOwnerCharacter() const {return Cast<APawn>(GetOwner());}

	UFUNCTION(NetMulticast, Client, Unreliable)
	void PlayMontageOnCharacter(UAnimMontage* AnimMontage, const float TimeToPlay = 0.f) const;
	UFUNCTION(NetMulticast, Client, Unreliable)
    void StopMontageOnCharacter() const;
	UFUNCTION(NetMulticast, Client, Unreliable)
	void PlayMontageOnWeapon(UAnimSequence* Animation, const float TimeToPlay = 0.f) const;
	

	void StartFire() const;
	void StopFire() const;

	void Reload() const;
	
	void StopAll() const;

	void SelectWeapon() const;


	//---------------------------------------------------------------------------------------------------------------------------
	//Setters
	void SetWeaponData(UBaseWeaponData* NewWeaponData);
	void SetWeaponMesh(USkeletalMesh* WeaponMesh) const {SkeletalMeshComponent->SetSkeletalMesh(WeaponMesh);}


	//---------------------------------------------------------------------------------------------------------------------------
	//Getters
	FName GetHandSocket() const {return WeaponData->HandSocketName;}

	UFUNCTION(BlueprintCallable)
	FCharacterWeaponAnimations GetAnimations() const {return WeaponAnimComponent->GetAnimations();}

	USkeletalMeshComponent* GetMesh() const {return SkeletalMeshComponent;}
	UWeaponAnimComponent* GetWeaponAnim() const {return WeaponAnimComponent;}
	UWeaponFXComponent* GetWeaponFX() const {return WeaponFXComponent;}
	
	
protected:
	
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	                                 FReplicationFlags* RepFlags) override;

private:
	static float CalculateMontageRate(UAnimSequenceBase* Animation, float TimeToPlay);
	
	UPROPERTY(EditAnywhere,	ReplicatedUsing = OnRep_WeaponData, Instanced, Category = "Weapon")
	UBaseWeaponData* WeaponData;

	UFUNCTION()
	void OnRep_WeaponData() const;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UWeaponFXComponent* WeaponFXComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UWeaponAnimComponent* WeaponAnimComponent = nullptr;
};


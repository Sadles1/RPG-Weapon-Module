// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFireMode.h"
#include "AutomaticFireMode.generated.h"


UCLASS()
class WEAPONMODULE_API UAutomaticFireMode : public UBaseFireMode
{
	GENERATED_BODY()



private:

	FTimerHandle AutomaticFireTimer;
	
	virtual void StartFire() override;
	virtual void StopFire() override;
};

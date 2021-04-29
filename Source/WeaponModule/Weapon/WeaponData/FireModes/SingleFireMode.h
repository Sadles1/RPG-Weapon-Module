// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFireMode.h"
#include "SingleFireMode.generated.h"

UCLASS()
class WEAPONMODULE_API USingleFireMode : public UBaseFireMode
{
	GENERATED_BODY()

public:

protected:

private:
	virtual void StartFire() override;


};

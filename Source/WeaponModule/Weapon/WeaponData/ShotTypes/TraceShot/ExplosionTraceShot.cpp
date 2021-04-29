// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionTraceShot.h"
#include "CharacterWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void UExplosionTraceShot::MakeDamage(TArray<FHitResult> HitResults)
{
	for( auto HitResult : HitResults)
	{
		FVector ExplosionPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
		
		UGameplayStatics::ApplyRadialDamage(GetWorld(), GetDamage(), ExplosionPoint, GetExplosionRadius(),
			                                    UDamageType::StaticClass(),
			                                    bDamageSelf
				                                    ? TArray<AActor*>{GetWeapon()}
				                                    : TArray<AActor*>{GetWeapon(), GetWeapon()->GetOwnerCharacter()},
			                                    GetWeapon(), GetWeapon()->GetOwnerCharacter()->GetController(), bDoFullDamage);
	}
}

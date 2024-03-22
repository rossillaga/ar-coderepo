// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	AttackAnimDelay = 0.2f;
	HandSocketName = "Muzzle_01";
}


void USAction_ProjectileAttack::AttackDelayElapsed(ACharacter* InstigatorCharacter)
{
	if(ensureAlways(ProjectileClass))
	{
		FHitResult Hit;
		FVector Start = InstigatorCharacter->GetPawnViewLocation();
		FVector End = Start + (InstigatorCharacter->GetControlRotation().Vector() * 10000.0f);
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
	
		if(GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams))
		{
			End = Hit.ImpactPoint;
		}
	
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		FRotator SpawnRotation = FRotationMatrix::MakeFromX(End - HandLocation).Rotator();
		FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);

		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if(Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelayElapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
	
}


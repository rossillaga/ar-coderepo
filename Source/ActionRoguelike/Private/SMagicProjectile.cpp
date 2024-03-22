// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SActionEffect.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	Damage = 20.0f;
}


void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != GetInstigator() && OtherActor)
	{

		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if(ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComponent->Velocity = -MovementComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			if(ActionComp && HasAuthority())
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			OnDestroyProjectile();
		}

	}
}

void ASMagicProjectile::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnCompHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	
	if(ensure(CameraShake))
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, Hit.Location, 200.0f, 1000.0 );
}






// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	MovementComponent->InitialSpeed = 6000.0f;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(Explosion_Timer, this, &ASDashProjectile::Explode, 0.2);
}

void ASDashProjectile::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ASDashProjectile::Explode()
{
	EffectComp->Deactivate();
	GetWorldTimerManager().SetTimer(Explosion_Timer, this, &ASDashProjectile::Teleport, 0.2);
	if(ensure(DestructionFX))
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionFX, GetActorLocation());
}

void ASDashProjectile::Teleport()
{
	MovementComponent->StopMovementImmediately();
	AActor* ActorToTeleport = GetInstigator();
	if(ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
	Destroy();
}

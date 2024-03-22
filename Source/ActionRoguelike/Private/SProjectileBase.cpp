// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	RootComponent = SphereComp;
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnCompHit);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("Audio Component");
	AudioComp->SetupAttachment(SphereComp);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComponent->InitialSpeed = 1000.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	
}

void ASProjectileBase::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(GetInstigator() != OtherActor)
	{
		OnDestroyProjectile();
	}
}

void ASProjectileBase::OnDestroyProjectile()
{
	if(ensure(DestructionFX))
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionFX, GetActorLocation(), GetActorRotation());
	if(ensure(ImpactSound))
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation());

	if(this)
		Destroy();
}


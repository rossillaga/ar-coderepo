// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("Barrel Mesh");
	RootComponent = BarrelMesh;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("Radial Force Component");
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	RadialForceComp->SetupAttachment(BarrelMesh);

	BarrelMesh->SetSimulatePhysics(true);
	
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	BarrelMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnCompHit);
}

void ASExplosiveBarrel::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetActorLocation());
	Destroy();
}




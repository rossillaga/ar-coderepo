// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerUp::ASPowerUp()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	BaseMesh->SetCollisionObjectType(ECC_WorldDynamic);
	RootComponent = BaseMesh;

	ReactivationTime = 10.0f;

	CreditCost = 50;

	SetReplicates(true);
}

void ASPowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerUp, bIsActive);
}

void ASPowerUp::OnRep_IsActive()
{
	BaseMesh->SetHiddenInGame(!bIsActive, true);
	SetActorEnableCollision(bIsActive);
}

void ASPowerUp::SetPowerupState(bool Active)
{
	bIsActive = Active;
	OnRep_IsActive();
}


// Called when the game starts or when spawned
void ASPowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	PowerUpFunction(InstigatorPawn);
}


FText ASPowerUp::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}
void ASPowerUp::PowerUpFunction(APawn* InstigatorPawn)
{
	FTimerHandle DeactivateTimer;
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(DeactivateTimer, this, &ASPowerUp::ReactivatePowerUp, ReactivationTime, false);
}

void ASPowerUp::ReactivatePowerUp()
{
	SetPowerupState(true);
}



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerUp.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerUp : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerUp();

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();

	void SetPowerupState(bool Active);
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere)
	float ReactivationTime;

	UPROPERTY(EditAnywhere, Category = "Defaults")
	int32 CreditCost;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Interact_Implementation(APawn* InstigatorPawn);

	virtual void PowerUpFunction(APawn* InstigatorPawn);

	UFUNCTION()
	void ReactivatePowerUp();

};



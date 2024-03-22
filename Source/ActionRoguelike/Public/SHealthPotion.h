// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUp.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPowerUp
{
	GENERATED_BODY()

public:
	ASHealthPotion();

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Defaults")
	float HealAmount;
	
	virtual void PowerUpFunction(APawn* InstigatorPawn) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUp.h"
#include "SPowerUp_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerUp_Credits : public ASPowerUp
{
	GENERATED_BODY()

	
protected:
	
	virtual void PowerUpFunction(APawn* InstigatorPawn) override;
};

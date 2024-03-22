// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUp.h"
#include "SPowerUp_Action.generated.h"

class USAction;

UCLASS()
class ACTIONROGUELIKE_API ASPowerUp_Action : public ASPowerUp
{
	GENERATED_BODY()

	
protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<USAction> ActionToGrant;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp_Credits.h"

#include "SPlayerState.h"

void ASPowerUp_Credits::PowerUpFunction(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	
	if(ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PS->AddCredits(CreditCost);
		Super::PowerUpFunction(InstigatorPawn);
	}
	
	
}

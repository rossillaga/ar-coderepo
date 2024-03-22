// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"
ASHealthPotion::ASHealthPotion()
{
	HealAmount = 20.0f;
	CreditCost = 50;
}

void ASHealthPotion::PowerUpFunction(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	if(USAttributeComponent* AC = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())))
	{
		if(AC->GetHealth() == 0 || AC->GetHealth() == AC->GetMaxHealth())
			return;
		if(ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if(PS->RemoveCredits(CreditCost) && !AttributeComp->IsAtFullHealth(InstigatorPawn))
			{
				AC->ApplyHealthChange(this, HealAmount);
				Super::PowerUpFunction(InstigatorPawn);
			}

		}
	}
}


FText ASHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	//return Super::GetInteractText_Implementation(InstigatorPawn);


	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	if(AttributeComp && AttributeComp->IsAtFullHealth(InstigatorPawn))
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health");
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores Health to Maximum"), CreditCost);
}

#undef LOCTEXT_NAMESPACE
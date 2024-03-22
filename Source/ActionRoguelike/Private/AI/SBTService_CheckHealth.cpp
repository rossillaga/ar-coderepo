// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth()
{
	HealthThreshold = 0.3f;
}

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		AAIController* MyControler = OwnerComp.GetAIOwner();
		if(ensure(MyControler))
		{
			APawn* AIPawn = MyControler->GetPawn();
			if(ensure(AIPawn))
			{
				USAttributeComponent* AC =Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

				if(AC)
				{
					if(AC->GetHealth() <= AC->GetMaxHealth() * HealthThreshold)
						bHasLowHealth = true;
					else
						bHasLowHealth = false;
				}

				BlackboardComp->SetValueAsBool(CheckHealthKey.SelectedKeyName, bHasLowHealth);
			}
		}
	}
}

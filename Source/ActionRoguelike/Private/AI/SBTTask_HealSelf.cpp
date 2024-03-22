// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if(ensure(MyController))
	{
		AActor* MyPawn = MyController->GetPawn();

		if(!MyPawn)
			return EBTNodeResult::Failed;

		USAttributeComponent* AC = USAttributeComponent::GetAttributes(MyPawn);
		if(AC)
		{
			AC->ApplyHealthChange(MyPawn, AC->GetMaxHealth());
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

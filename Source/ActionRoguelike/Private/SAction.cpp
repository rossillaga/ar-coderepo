// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"


void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
	DOREPLIFETIME(USAction, TimeStarted);
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		return false;
	}
	
	USActionComponent* Comp = GetOwningComponent();
	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{

	//UE_LOG(LogTemp, Warning, TEXT("Running : %s"), *GetNameSafe(this))
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if(GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
		TimeStarted = GetWorld()->TimeSeconds;
	
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Warning, TEXT("Stopped : %s"), *GetNameSafe(this))
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	//ensureAlways(bIsRunning);
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

UWorld* USAction::GetWorld() const
{
	 AActor* Actor = Cast<AActor>(GetOuter());

	if(Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}
USActionComponent* USAction::GetOwningComponent() const
{
	
	//return Cast<USActionComponent>(GetOuter());
	return ActionComp;
}

void USAction::OnRep_Repdata()
{
	if(RepData.bIsRunning)
		StartAction(RepData.Instigator);
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool USAction::IsRunning() const
{

	return RepData.bIsRunning;
}



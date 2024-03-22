// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SWorldUserWidget.h"
#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"


static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("jl.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for interact Component."), ECVF_Cheat);
// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
}


void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if(MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
	
}

void USInteractionComponent::PrimaryInteract()
{

	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if(!InFocus)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No focus actor");
		return;
	}
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}

void USInteractionComponent::FindBestInteractable()
{
	
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();



	FVector Eyelocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(Eyelocation, EyeRotation);
	FVector End = Eyelocation + (EyeRotation.Vector() * TraceDistance);
	//FHitResult Hit;
	
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, Eyelocation, End, ObjectQueryParams);
	
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Eyelocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(bDebugDraw)
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
			if(HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if(FocusedActor)
	{
		if(!DefaultWidgetInstance && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			
			if(!DefaultWidgetInstance->IsInViewport())
				DefaultWidgetInstance->AddToViewport();
		}
		
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if(bDebugDraw)
		DrawDebugLine(GetWorld(), Eyelocation, End, LineColor, false ,2.0f, 2.0f);
	
}

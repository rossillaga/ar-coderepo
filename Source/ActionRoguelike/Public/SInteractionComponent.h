// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


class USWorldUserWidget;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract();

protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
	void FindBestInteractable();

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float TraceRadius;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;
	
public:	
	// Sets default values for this component's properties
	USInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

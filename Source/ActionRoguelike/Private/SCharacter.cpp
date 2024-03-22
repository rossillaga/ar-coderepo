// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"


#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("Interaction Component");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("Attribute Component");

	ActionComp = CreateDefaultSubobject<USActionComponent>("Action Component");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("FirstSpell", IE_Pressed, this, &ASCharacter::BlackHoleAbility);
	PlayerInputComponent->BindAction("SecondSpell", IE_Pressed, this, &ASCharacter::DashAbility);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);

}

void ASCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}


// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnAttributeChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ASCharacter::MoveForward(float Value)
{
	if(Value)
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0;
		ControlRot.Roll = 0;
		AddMovementInput(ControlRot.Vector(), Value);
	}
}

void ASCharacter::MoveRight(float Value)
{	if(Value)
	{
		FRotator ControlRot = GetControlRotation();
		ControlRot.Pitch = 0;
		ControlRot.Roll = 0;
		FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
		AddMovementInput(RightVector, Value);
	}
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::PrimaryInteract()
{
	if(InteractionComp)
		InteractionComp->PrimaryInteract();
}

void ASCharacter::BlackHoleAbility()
{
	ActionComp->StartActionByName(this, "BlackHole");
}

void ASCharacter::DashAbility()
{
	ActionComp->StartActionByName(this, "Dash");
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{

	if(Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
		float RageDelta = FMath::Abs(Delta/2);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
		
		if(NewHealth <= 0)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);

			SetLifeSpan(5.0f);
		}
	}
	
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


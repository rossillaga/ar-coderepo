// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
class ASPowerUp;
class USSaveGame;
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;
	
	UPROPERTY()
	USSaveGame* CurrentSaveGame;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	FTimerHandle TimerHandle_SpawnBots;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 CreditsPerKill;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<ASPowerUp>> PowerUpClasses;
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UEnvQuery* PowerUpSpawnQuery;
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float RequiredPowerupDistance {500};
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 DesiredPowerupCount {5};

	UFUNCTION()
	void SpawnBotTimerElapsed();
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	ASGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void WriteSavedGame();

	void LoadSaveGame();
	
};

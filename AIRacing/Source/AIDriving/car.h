// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "neuralNetwork.h"

// input
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"

#include "car.generated.h"

UCLASS()
class AIDRIVING_API Acar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Acar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void bpBeginPlay();
	UFUNCTION(BlueprintCallable)
	void setupNetwork(int networkIndex);

	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void callFeedForward(const TArray<float>& distances, float angle, float speed);

	// tells the program how good the car did
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	float calcFitness();

	UFUNCTION(BlueprintCallable)
	void destroyCar();
	UFUNCTION(BlueprintImplementableEvent)
	void bpDestroyCar();

	UFUNCTION(BlueprintImplementableEvent)
	void spawnSkidMarks(bool skidMarks);
	
	neuralNetwork* network;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintImplementableEvent)
	void bpTick(float deltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	TArray<FLayersWeights> getWeights();
	UFUNCTION(BlueprintCallable)
	TArray<FLayersBiases> getBiases();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* carMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int points = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int populationSize = 25;

	static inline int generation = 0;

	UFUNCTION(BlueprintCallable)
	int getGeneration();

	static inline TArray<FNetworkData> networkData;

	// make sure it can't hit the ring multiple times
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<AActor*> pointRingsAlreadyHit;
	// kill after certain time if hasn't hit ring
	float deathTimer = 5.f; // dies after 5 seconds
	float deathTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int lapNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int id = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAlive = true;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "neuralNetwork.h"
#include "MyPlayerController.generated.h"
/**
 * 
 */
UCLASS()
class AIDRIVING_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	void delaySetupNextGeneration(int& generation, TArray<FNetworkData>& networkData, int populationSize, neuralNetwork* network);
	void setupNextGeneration(int& generation, TArray<FNetworkData>& networkData, int populationSize, neuralNetwork* network);
	UFUNCTION(BlueprintImplementableEvent)
	void spawnNextGeneration(int carIndex, bool isBest);
};
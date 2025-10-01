// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "neuralNetwork.h"
#include "mySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class AIDRIVING_API UmySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void setSaveVariables();

	UFUNCTION(BlueprintCallable)
	void giveSaveVariables();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FNetworkData> networkData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int generation;
};

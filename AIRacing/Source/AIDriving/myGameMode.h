// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "myGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AIDRIVING_API AmyGameMode : public AGameMode
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
};

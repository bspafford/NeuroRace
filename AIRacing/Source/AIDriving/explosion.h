// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "explosion.generated.h"

UCLASS()
class AIDRIVING_API Aexplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aexplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	UMediaPlayer* createMediaPlayer();
	UFUNCTION(BlueprintCallable)
	UMediaTexture* createMediaTexture();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMediaPlayer* mediaPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMediaTexture* mediaTexture;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SplineComponent.h"

#include "car.h"

#include "camera.generated.h"

UCLASS()
class AIDRIVING_API Acamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Acamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components");
	USpringArmComponent* springArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components");
	UCameraComponent* camera;

	UFUNCTION(BlueprintCallable)
	TArray<Acar*> sortCarList(TArray<Acar*> carList, USplineComponent* spline);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

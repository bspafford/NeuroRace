// Fill out your copyright notice in the Description page of Project Settings.


#include "camera.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
Acamera::Acamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
}

// Called when the game starts or when spawned
void Acamera::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
		PC->SetViewTargetWithBlend(this, 0.f);
}

// Called every frame
void Acamera::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

TArray<Acar*> Acamera::sortCarList(TArray<Acar*> carList, USplineComponent* spline) {

	// get most laps made
	int mostLaps = 0;
	for (int i = 0; i < carList.Num(); i++) {
		if (carList[i]->lapNum > mostLaps)
			mostLaps = carList[i]->lapNum;
	}

	// remove other cars that don't have the most laps
	for (int i = carList.Num() - 1; i >= 0; i--) {
		if (carList[i]->lapNum != mostLaps) {
			carList.RemoveAt(i);
		}
	}

	carList.Sort([spline](const AActor& A, const AActor& B) {
		return spline->GetDistanceAlongSplineAtLocation(A.GetActorLocation(), ESplineCoordinateSpace::World) < spline->GetDistanceAlongSplineAtLocation(B.GetActorLocation(), ESplineCoordinateSpace::World);
	});

	return carList;
}

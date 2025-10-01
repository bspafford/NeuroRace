// Fill out your copyright notice in the Description page of Project Settings.


#include "car.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/RandomShuffle.h"
#include "Async/ParallelFor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SplineComponent.h"
#include "MyPlayerController.h"

// Sets default values
Acar::Acar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	carMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car Mesh"));
	carMesh->SetupAttachment(RootComponent);

	// load mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> carAsset(TEXT("/Game/car.car"));
	if (carAsset.Succeeded())
		carMesh->SetStaticMesh(carAsset.Object);
	carMesh->SetWorldLocation(FVector3d(16.f, 0, -80.f));

	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCapsuleSize(30.f, 30.f);
}

// Called when the game starts or when spawned
void Acar::BeginPlay()
{
	Super::BeginPlay();

	if (!Controller)
		SpawnDefaultController();

	if (APlayerController* PC = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	bAlive = true;

	bpBeginPlay();
}

void Acar::setupNetwork(int networkIndex) {
	//if (networkIndex == 0 || networkIndex == 1)
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::FromInt(networkData.Num()));

	id = networkIndex;

	network = new neuralNetwork();
	if (networkIndex >= networkData.Num() || networkData.Num() == 0) {
		network->InitializeNetwork(7, 2, 2);
		networkData.Add(FNetworkData(0, network->getWeights(), network->getBiases()));
	} else {
		network->InitializeNetwork(networkData[networkIndex].weights, networkData[networkIndex].biases);
	}
}

// Called every frame
void Acar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// resets in pointRing on overlap
	deathTime += DeltaTime;
	if (deathTime >= deathTimer)
		destroyCar();

	if (GetRootComponent()->GetComponentLocation().Z < -20.f) {
		destroyCar();
	}

	bpTick(DeltaTime);
}

// Called to bind functionality to input
void Acar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Acar::Move);
	}

}

void Acar::Move(const FInputActionValue& Value) {
	FVector2D MovementVector = Value.Get<FVector2D>();

	//GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Green, Value.ToString());

	if (Controller) {
		FRotator deltaRotation = FRotator(0.f, 90.f * MovementVector.Y * GetWorld()->GetDeltaSeconds(), 0.f);

		// drifting
		FVector velocityDir = GetVelocity().GetSafeNormal2D();
		FVector forwardDir = GetActorForwardVector().GetSafeNormal2D();
		float driftAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(velocityDir, forwardDir)));
		if (driftAngle > 15.f)
			spawnSkidMarks(true);
		else
			spawnSkidMarks(false);

		AddActorLocalRotation(deltaRotation * 2.f);

		AddMovementInput(GetActorForwardVector(), MovementVector.X);
	}
}

void Acar::callFeedForward(const TArray<float>& distances, float angle, float speed) {
	TArray<float> inputs = distances;
	inputs.Add(angle);
	inputs.Add(speed);
	if (network) {
		TArray<float> outputs = network->feedForward(inputs);

		float turning = outputs[1];
		float accelerate = outputs[0];

		FRotator deltaRotation = FRotator(0.f, 90.f * turning * GetWorld()->GetDeltaSeconds(), 0.f);

		// drifting
		FVector velocityDir = GetVelocity().GetSafeNormal2D();
		FVector forwardDir = GetActorForwardVector().GetSafeNormal2D();
		float driftAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(velocityDir, forwardDir)));
		if (driftAngle > 15.f)
			spawnSkidMarks(true);
		else
			spawnSkidMarks(false);

		AddActorLocalRotation(deltaRotation * 3.f);

		AddMovementInput(GetActorForwardVector(), accelerate);
	}
}

void Acar::destroyCar() {
	if (!bAlive) {
		return;
	}

	if (id == -1 || !network) {
		bpDestroyCar();
		bAlive = false;
		GetCharacterMovement()->Velocity = FVector::Zero();
		return;
	}

	float fitness = calcFitness();

	if (network)
		network->setFitness(fitness);

	if (id < networkData.Num())
		networkData[id].fitness = fitness;

	// is last car to die
	TArray<AActor*> cars;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Acar::StaticClass(), cars);

	bAlive = false;

	bool allDead = true;
	for (auto actor : cars) {
		auto car = Cast<Acar>(actor);
		if (car && car->bAlive) {
			allDead = false;
			break;
		}
	}

	if (allDead || cars.Num() == 0) {
		AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (MyPC)
			MyPC->delaySetupNextGeneration(generation, networkData, populationSize, network);
	}

	bpDestroyCar();
	GetCharacterMovement()->Velocity = FVector::Zero();
}

int Acar::getGeneration() {
	return generation;
}

TArray<FLayersWeights> Acar::getWeights() {
	if (network)
		return network->weights;
	return TArray<FLayersWeights>{};
}

TArray<FLayersBiases> Acar::getBiases() {
	if (network)
		return network->biases;
	return TArray<FLayersBiases>{};
}

//void Acar::calcFitness() {
	/*
	based on
		distance traveled, distance along spline (0 - 1) + laps
		time alive
		average speed
		staying close to center of road


	then keep top X%
	example 20%
	you would make 80% of next gen = children from crossover/mutation
	and 20% = exact copies (elitism)
	*/
//}

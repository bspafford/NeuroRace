// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "car.h"
#include "neuralNetwork.h"
#include "Algo/RandomShuffle.h"

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AMyPlayerController::delaySetupNextGeneration(int& generation, TArray<FNetworkData>& networkData, int populationSize, neuralNetwork* network) {
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle,
		[this, &generation, &networkData, populationSize, network]() {
			setupNextGeneration(generation, networkData, populationSize, network);
		}, 1.5f, false);
}

void AMyPlayerController::setupNextGeneration(int &generation, TArray<FNetworkData> &networkData, int populationSize, neuralNetwork* network) {
	generation++;

	// get fitness that are top 20%
	float percentile = .2;
	// sort list
	networkData.Sort([](const FNetworkData& A, const FNetworkData& B) {
		return A.fitness > B.fitness; // Descending
		});

	TArray<FNetworkData> newGeneration;
	int topCount = FMath::CeilToInt(networkData.Num() * percentile);
	// exact replicas
	TArray<FNetworkData> topCars;
	for (int i = 0; i < topCount; i++) {
		topCars.Add(networkData[i]);
		newGeneration.Add(networkData[i]);
	}

	float bestFitness = topCars.Num() != 0 ? topCars[0].fitness : 0;

	// crossover and mutation
	Algo::RandomShuffle(topCars); // randomly shuffle for more diverse pairs

	while (newGeneration.Num() < populationSize && topCars.Num() != 0) {
		if (topCars.Num() == 1) {
			FNetworkData child = topCars[0];
			network->Mutate(child);
			newGeneration.Add(child);
			continue;
		}

		float indexA = FMath::RandRange(0, topCars.Num() - 1);
		float indexB = FMath::RandRange(0, topCars.Num() - 1);

		if (indexA == indexB)
			continue;

		// pick two random parents
		FNetworkData parentA = topCars[indexA];
		FNetworkData parentB = topCars[indexB];

		FNetworkData child = network->crossover(parentA, parentB);
		network->Mutate(child);

		newGeneration.Add(child);
	}

	networkData = newGeneration;

	// spawn the new generation
	for (int i = 0; i < newGeneration.Num(); i++) {
		bool isBest = newGeneration[i].fitness == bestFitness;
		spawnNextGeneration(i, isBest);
	}
}